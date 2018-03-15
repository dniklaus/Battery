/*
 * BatteryImpl.cpp
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

// #include "Arduino.h"

#include "Timer.h"
#include "Battery.h"
#include "BatteryVoltageEvalFsm.h"
#include "BatteryImpl.h"

//-----------------------------------------------------------------------------

class BattStartupTimerAdapter : public TimerAdapter
{
private:
  BatteryImpl* m_battImpl;

public:
  BattStartupTimerAdapter(BatteryImpl* battImpl)
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

class BattPollTimerAdapter : public TimerAdapter
{
private:
  BatteryImpl* m_battImpl;

public:
  BattPollTimerAdapter(BatteryImpl* battImpl)
  : m_battImpl(battImpl)
  { }

  void timeExpired()
  {
    if (0 != m_battImpl)
    {
      // Serial.println("BattPollTimerAdapter::timeExpired()");
      m_battImpl->evaluateStatus();
    }
  }
};

//-----------------------------------------------------------------------------

const unsigned int BatteryImpl::s_DEFAULT_STARTUP_TIME = 500;
const unsigned int BatteryImpl::s_DEFAULT_POLL_TIME = 1000;

BatteryImpl::BatteryImpl(BatteryAdapter* adapter, BatteryThresholdConfig batteryThresholdConfig)
: m_adapter(adapter)
, m_evalFsm(new BatteryVoltageEvalFsm(this))
, m_startupTimer(new Timer(new BattStartupTimerAdapter(this), Timer::IS_NON_RECURRING, s_DEFAULT_STARTUP_TIME))
, m_pollTimer(new Timer(new BattPollTimerAdapter(this), Timer::IS_RECURRING))
, m_batteryVoltage(0.0)
, m_battVoltageSenseFactor(2.0)
, m_battWarnThreshd(batteryThresholdConfig.battWarnThreshd)
, m_battStopThrshd(batteryThresholdConfig.battStopThrshd)
, m_battShutThrshd(batteryThresholdConfig.battShutThrshd)
, m_battHyst(batteryThresholdConfig.battHyst)
{ }

BatteryImpl::~BatteryImpl()
{
  delete m_pollTimer->adapter();
  delete m_pollTimer; m_pollTimer = 0;

  delete m_startupTimer->adapter();
  delete m_startupTimer; m_startupTimer = 0;

  delete m_evalFsm;

  m_adapter = 0;
}

void BatteryImpl::attachAdapter(BatteryAdapter* adapter)
{
  m_adapter = adapter;
  m_evalFsm->attachAdapter(m_adapter);
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
  m_pollTimer->startTimer(s_DEFAULT_POLL_TIME);
}

void BatteryImpl::evaluateStatus()
{
  if ((0 != m_adapter) && (0 != m_evalFsm))
  {
    float batteryVoltage = m_adapter->readRawBattSenseValue() * m_battVoltageSenseFactor * adapter()->getVAdcFullrange() / adapter()->getNAdcFullrange();
    m_batteryVoltage = batteryVoltage;
    // Serial.print("BatteryImpl::evaluateStatus(), m_batteryVoltage = ");
    // Serial.print(m_batteryVoltage);
    // Serial.println(" V");
    m_evalFsm->evaluateStatus();
  }
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
