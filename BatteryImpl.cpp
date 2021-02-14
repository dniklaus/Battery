/*
 * BatteryImpl.cpp
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#include "SpinTimer.h"
#include "Battery.h"
#include "BatteryVoltageEvalFsm.h"
#include "BatteryImpl.h"

//-----------------------------------------------------------------------------

class BattStartupTimerAction : public SpinTimerAction
{
private:
  BatteryImpl* m_battImpl;

public:
  BattStartupTimerAction(BatteryImpl* battImpl)
  : m_battImpl(battImpl)
  { }

  void timeExpired()
  {
    if (0 != m_battImpl)
    {
      m_battImpl->startup();
    }
  }
};

//-----------------------------------------------------------------------------

class BattStatusEvalTimerAction : public SpinTimerAction
{
private:
  BatteryImpl* m_battImpl;

public:
  BattStatusEvalTimerAction(BatteryImpl* battImpl)
  : m_battImpl(battImpl)
  { }

  void timeExpired()
  {
    if (0 != m_battImpl)
    {
      m_battImpl->evaluateStatus();
    }
  }
};

//-----------------------------------------------------------------------------

const unsigned int BatteryImpl::s_DEFAULT_STARTUP_TIME = 500;
const unsigned int BatteryImpl::s_DEFAULT_POLL_TIME = 5000;
const unsigned int BatteryImpl::s_DEFAULT_ASYNC_STATUS_EVAL_TIME = 0;

BatteryImpl::BatteryImpl(BatteryAdapter* adapter, BatteryThresholdConfig batteryThresholdConfig)
: m_adapter(adapter)
, m_evalFsm(new BatteryVoltageEvalFsm(this))
, m_startupTimer(new SpinTimer(s_DEFAULT_STARTUP_TIME, new BattStartupTimerAction(this), SpinTimer::IS_NON_RECURRING, SpinTimer::IS_AUTOSTART))
, m_pollTimer(new SpinTimer(s_DEFAULT_POLL_TIME, new BattStatusEvalTimerAction(this), SpinTimer::IS_RECURRING, SpinTimer::IS_NON_AUTOSTART))
, m_evalStatusTimer(new SpinTimer(s_DEFAULT_ASYNC_STATUS_EVAL_TIME, m_pollTimer->action(), SpinTimer::IS_NON_RECURRING, SpinTimer::IS_NON_AUTOSTART))   // re-use the same BattStatusEvalTimerAdapter object
, m_batteryVoltage(0.0)
, m_battVoltageSenseFactor(2.0)
, m_battWarnThreshd(batteryThresholdConfig.battWarnThreshd)
, m_battStopThrshd(batteryThresholdConfig.battStopThrshd)
, m_battShutThrshd(batteryThresholdConfig.battShutThrshd)
, m_battHyst(batteryThresholdConfig.battHyst)
{ }

BatteryImpl::~BatteryImpl()
{
  delete m_evalStatusTimer;
  m_evalStatusTimer = 0;

  delete m_pollTimer->action();
  delete m_pollTimer; m_pollTimer = 0;

  delete m_startupTimer->action();
  delete m_startupTimer; m_startupTimer = 0;

  delete m_evalFsm;

  m_adapter = 0;
}

void BatteryImpl::attachAdapter(BatteryAdapter* adapter)
{
  m_adapter = adapter;
  m_evalFsm->attachAdapter(m_adapter);
  battVoltageSensFactorChanged();
}

BatteryAdapter* BatteryImpl::adapter()
{
  return m_adapter;
}

void BatteryImpl::startup()
{
  if (0 != m_adapter)
  {
    m_battVoltageSenseFactor = m_adapter->readBattVoltageSenseFactor();
  }
  evaluateStatusAsync();
  m_pollTimer->start(s_DEFAULT_POLL_TIME);
}

void BatteryImpl::evaluateStatus()
{
  if ((0 != m_adapter) && (0 != m_evalFsm))
  {
    m_batteryVoltage = m_adapter->readRawBattSenseValue() * m_battVoltageSenseFactor * adapter()->getVAdcFullrange() / (adapter()->getNAdcFullrange() + 1);
    m_evalFsm->evaluateStatus();
  }
}

void BatteryImpl::evaluateStatusAsync()
{
  m_evalStatusTimer->start(s_DEFAULT_ASYNC_STATUS_EVAL_TIME);
}

void BatteryImpl::battVoltageSensFactorChanged()
{
  if (0 != m_adapter)
  {
    m_battVoltageSenseFactor = m_adapter->readBattVoltageSenseFactor();
  }
}

float BatteryImpl::getBatteryVoltage()
{
  return m_batteryVoltage;
}

bool BatteryImpl::isBattVoltageOk()
{
  bool isVoltageOk = false;
  if (0 != m_evalFsm)
  {
    isVoltageOk = m_evalFsm->isBattVoltageOk();
  }
  return isVoltageOk;
}

bool BatteryImpl::isBattVoltageBelowWarnThreshold()
{
  bool isVoltageBelowWarnThreshold = true;
  if (0 != m_evalFsm)
  {
    isVoltageBelowWarnThreshold = m_evalFsm->isBattVoltageBelowWarnThreshold();
  }
  return isVoltageBelowWarnThreshold;
}

bool BatteryImpl::isBattVoltageBelowStopThreshold()
{
  bool isVoltageBelowStopThreshold = true;
  if (0 != m_evalFsm)
  {
    isVoltageBelowStopThreshold = m_evalFsm->isBattVoltageBelowStopThreshold();
  }
  return isVoltageBelowStopThreshold;
}

bool BatteryImpl::isBattVoltageBelowShutdownThreshold()
{
  bool isVoltageBelowShutdownThreshold = true;
  if (0 != m_evalFsm)
  {
    isVoltageBelowShutdownThreshold = m_evalFsm->isBattVoltageBelowShutdownThreshold();
  }
  return isVoltageBelowShutdownThreshold;
}

const char* BatteryImpl::getCurrentStateName()
{
  if (0 == m_evalFsm)
  {
    return "BatteryImpl::m_evalFsm, null pointer exception";
  }
  return m_evalFsm->state()->toString();
}

const char* BatteryImpl::getPreviousStateName()
{
  if (0 == m_evalFsm)
  {
    return "BatteryImpl::m_evalFsm, null pointer exception";
  }
  return m_evalFsm->previousState()->toString();
}

float BatteryImpl::battWarnThreshd()
{
  return m_battWarnThreshd;
}

float BatteryImpl::battStopThrshd()
{
  return m_battStopThrshd;
}

float BatteryImpl::battShutThrshd()
{
  return m_battShutThrshd;
}

float BatteryImpl::battHyst()
{
  return m_battHyst;
}
