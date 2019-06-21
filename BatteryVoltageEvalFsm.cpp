/*
 * BatteryVoltageEvalFsm.cpp
 *
 *  Created on: 16.05.2014
 *      Author: niklausd
 */

#include "BatteryImpl.h"
#include "Battery.h"
#include "BatteryVoltageEvalFsm.h"

BatteryVoltageEvalFsm::BatteryVoltageEvalFsm(BatteryImpl* battImpl)
: m_battImpl(battImpl)
, m_adapter(battImpl->adapter())
, m_state(BatteryVoltageEvalFsmState_BattUnknown::Instance())
, m_previousState(BatteryVoltageEvalFsmState_BattUnknown::Instance())
{ }

BatteryVoltageEvalFsm::~BatteryVoltageEvalFsm()
{
  m_state = 0;
  m_adapter = 0;
  m_battImpl = 0;
}

void BatteryVoltageEvalFsm::attachAdapter(BatteryAdapter* adapter)
{
  m_adapter = adapter;
}

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsm::state()
{
  return m_state;
}

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsm::previousState()
{
  return m_previousState;
}

BatteryAdapter* BatteryVoltageEvalFsm::adapter()
{
  return m_adapter;
}

void BatteryVoltageEvalFsm::changeState(BatteryVoltageEvalFsmState* state)
{
  m_previousState = m_state;
  m_state = state;
  if (0 != state)
  {
    state->entry(this);
  }
}

void BatteryVoltageEvalFsm::evaluateStatus()
{
  if ((0 != m_state) && (0 != m_adapter))
  {
    m_state->evaluateState(this);
  }
}

bool BatteryVoltageEvalFsm::isBattVoltageOk()
{
  return (BatteryVoltageEvalFsmState_BattOk::Instance() == m_state);
}

bool BatteryVoltageEvalFsm::isBattVoltageBelowWarnThreshold()
{
  bool isBattVoltageBelowWarnThreshold = (
      (BatteryVoltageEvalFsmState_BattVoltageBelowWarn::Instance()      == m_state) ||
      (BatteryVoltageEvalFsmState_BattVoltageBelowStop::Instance()      == m_state) ||
      (BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance()  == m_state));
  return isBattVoltageBelowWarnThreshold;
}

bool BatteryVoltageEvalFsm::isBattVoltageBelowStopThreshold()
{
  bool isBattVoltageBelowStopThreshold = (
      (BatteryVoltageEvalFsmState_BattVoltageBelowStop::Instance()      == m_state) ||
      (BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance()  == m_state));
  return isBattVoltageBelowStopThreshold;
}

bool BatteryVoltageEvalFsm::isBattVoltageBelowShutdownThreshold()
{
  bool isBattVoltageBelowShutdownThreshold = (
      (BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance()  == m_state));
  return isBattVoltageBelowShutdownThreshold;
}

bool BatteryVoltageEvalFsm::isGuardWarn()
{
  bool isGuard = false;
  if (0 != m_battImpl)
  {
    isGuard = (m_battImpl->battWarnThreshd() > m_battImpl->getBatteryVoltage());
  }
  return isGuard;
}

bool BatteryVoltageEvalFsm::isGuardStop()
{
  bool isGuard = false;
  if (0 != m_battImpl)
  {
    isGuard = (m_battImpl->battStopThrshd() > m_battImpl->getBatteryVoltage());
  }
  return isGuard;
}

bool BatteryVoltageEvalFsm::isGuardShut()
{
  bool isGuard = false;
  if (0 != m_battImpl)
  {
    isGuard = (m_battImpl->battShutThrshd() > m_battImpl->getBatteryVoltage());
  }
  return isGuard;
}

bool BatteryVoltageEvalFsm::isGuardWarnPlusHyst()
{
  bool isGuard = false;
  if (0 != m_battImpl)
  {
    isGuard = ((m_battImpl->battWarnThreshd() + m_battImpl->battHyst()) < m_battImpl->getBatteryVoltage());
  }
  return isGuard;
}

bool BatteryVoltageEvalFsm::isGuardStopPlusHyst()
{
  bool isGuard = false;
  if (0 != m_battImpl)
  {
    isGuard = ((m_battImpl->battStopThrshd() + m_battImpl->battHyst()) < m_battImpl->getBatteryVoltage());
  }
  return isGuard;
}

bool BatteryVoltageEvalFsm::isGuardShutPlusHyst()
{
  bool isGuard = false;
  if (0 != m_battImpl)
  {
    isGuard = ((m_battImpl->battShutThrshd() + m_battImpl->battHyst()) < m_battImpl->getBatteryVoltage());
  }
  return isGuard;
}

//-----------------------------------------------------------------------------

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattUnknown::s_instance = 0;

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattUnknown::Instance()
{
  if (0 == s_instance)
  {
    s_instance = new BatteryVoltageEvalFsmState_BattUnknown();
  }
  return s_instance;
}

const char* BatteryVoltageEvalFsmState_BattUnknown::toString()
{
  return "BattUnknown";
}

void BatteryVoltageEvalFsmState_BattUnknown::evaluateState(BatteryVoltageEvalFsm* fsm)
{
  if (0 != fsm)
  {
    if (fsm->isGuardWarnPlusHyst())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattOk::Instance());
    }
    else if (fsm->isGuardWarn())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowWarn::Instance());
    }
    else if (fsm->isGuardStop())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowStop::Instance());
    }
    else if (fsm->isGuardShut())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance());
    }
  }
}

void BatteryVoltageEvalFsmState_BattUnknown::entry(BatteryVoltageEvalFsm* fsm)
{ }

//-----------------------------------------------------------------------------

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattOk::s_instance = 0;

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattOk::Instance()
{
  if (0 == s_instance)
  {
    s_instance = new BatteryVoltageEvalFsmState_BattOk();
  }
  return s_instance;
}

const char* BatteryVoltageEvalFsmState_BattOk::toString()
{
  return "BattOk";
}

void BatteryVoltageEvalFsmState_BattOk::evaluateState(BatteryVoltageEvalFsm* fsm)
{
  if (0 != fsm)
  {
    if (fsm->isGuardWarn())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowWarn::Instance());
    }
  }
}

void BatteryVoltageEvalFsmState_BattOk::entry(BatteryVoltageEvalFsm* fsm)
{
  if ((0 != fsm) && (0 != fsm->adapter()))
  {
    fsm->adapter()->notifyBattVoltageOk();
  }
}

//-----------------------------------------------------------------------------

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattVoltageBelowWarn::s_instance = 0;

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattVoltageBelowWarn::Instance()
{
  if (0 == s_instance)
  {
    s_instance = new BatteryVoltageEvalFsmState_BattVoltageBelowWarn();
  }
  return s_instance;
}

const char* BatteryVoltageEvalFsmState_BattVoltageBelowWarn::toString()
{
  return "BattVoltageBelowWarn";
}

void BatteryVoltageEvalFsmState_BattVoltageBelowWarn::evaluateState(BatteryVoltageEvalFsm* fsm)
{
  if (0 != fsm)
  {
    if (fsm->isGuardStop())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowStop::Instance());
    }
    else if (fsm->isGuardWarnPlusHyst())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattOk::Instance());
    }
  }
}

void BatteryVoltageEvalFsmState_BattVoltageBelowWarn::entry(BatteryVoltageEvalFsm* fsm)
{
  if ((0 != fsm) && (0 != fsm->adapter()))
  {
    fsm->adapter()->notifyBattVoltageBelowWarnThreshold();
  }
}

//-----------------------------------------------------------------------------

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattVoltageBelowStop::s_instance = 0;

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattVoltageBelowStop::Instance()
{
  if (0 == s_instance)
  {
    s_instance = new BatteryVoltageEvalFsmState_BattVoltageBelowStop();
  }
  return s_instance;
}

const char* BatteryVoltageEvalFsmState_BattVoltageBelowStop::toString()
{
  return "BattVoltageBelowStop";
}

void BatteryVoltageEvalFsmState_BattVoltageBelowStop::evaluateState(BatteryVoltageEvalFsm* fsm)
{
  if (0 != fsm)
  {
    if (fsm->isGuardShut())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance());
    }
    else if (fsm->isGuardStopPlusHyst())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowWarn::Instance());
    }
  }
}

void BatteryVoltageEvalFsmState_BattVoltageBelowStop::entry(BatteryVoltageEvalFsm* fsm)
{
  if ((0 != fsm) && (0 != fsm->adapter()))
  {
    fsm->adapter()->notifyBattVoltageBelowStopThreshold();
  }
}

//-----------------------------------------------------------------------------

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::s_instance = 0;

BatteryVoltageEvalFsmState* BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance()
{
  if (0 == s_instance)
  {
    s_instance = new BatteryVoltageEvalFsmState_BattVoltageBelowShutdown();
  }
  return s_instance;
}

const char* BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::toString()
{
  return "BattVoltageBelowShutdown";
}

void BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::evaluateState(BatteryVoltageEvalFsm* fsm)
{
  if (0 != fsm)
  {
    if (fsm->isGuardShutPlusHyst())
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowWarn::Instance());
    }
    else
    {
      fsm->changeState(BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::Instance());
    }
  }
}

void BatteryVoltageEvalFsmState_BattVoltageBelowShutdown::entry(BatteryVoltageEvalFsm* fsm)
{
  if ((0 != fsm) && (0 != fsm->adapter()))
  {
    fsm->adapter()->notifyBattVoltageBelowShutdownThreshold();
  }
}

//-----------------------------------------------------------------------------
