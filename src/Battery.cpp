/*
 * Battery.cpp
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#include "../Battery.h"
#include "BatteryImpl.h"

Battery::Battery(BatteryAdapter* adapter)
: m_impl(new BatteryImpl(adapter))
{ }

Battery::~Battery()
{
  delete adapter();
  delete m_impl; m_impl = 0;
}

void Battery::attachAdapter(BatteryAdapter* adapter)
{
  if (0 != m_impl)
  {
    m_impl->attachAdapter(adapter);
  }
}

BatteryAdapter* Battery::adapter()
{
  BatteryAdapter* adapter = 0;
  if (0 != m_impl)
  {
    adapter = m_impl->adapter();
  }
  return adapter;
}

const char* Battery::getCurrentStateName()
{
  if (0 == m_impl)
  {
    return "Battery::m_impl, null pointer exception";
  }
  return m_impl->getCurrentStateName();
}

const char* Battery::getPreviousStateName()
{
  if (0 == m_impl)
  {
    return "Battery::m_impl, null pointer exception";
  }
  return m_impl->getPreviousStateName();
}

void Battery::battVoltageSensFactorChanged()
{
  if (0 != m_impl)
  {
    m_impl->battVoltageSensFactorChanged();
  }
}

float Battery::getBatteryVoltage()
{
  float batteryVoltage = 0.0;
  if (0 != m_impl)
  {
    batteryVoltage = m_impl->getBatteryVoltage();
  }
  return batteryVoltage;
}

bool Battery::isBattVoltageOk()
{
  bool isVoltageOk = true;
  if (0 != m_impl)
  {
    isVoltageOk = m_impl->isBattVoltageOk();
  }
  return isVoltageOk;
}

bool Battery::isBattVoltageBelowWarnThreshold()
{
  bool isVoltageBelowWarnThreshold = false;
  if (0 != m_impl)
  {
    isVoltageBelowWarnThreshold = m_impl->isBattVoltageBelowWarnThreshold();
  }
  return isVoltageBelowWarnThreshold;
}

bool Battery::isBattVoltageBelowStopThreshold()
{
  bool isVoltageBelowStopThreshold = false;
  if (0 != m_impl)
  {
    isVoltageBelowStopThreshold = m_impl->isBattVoltageBelowStopThreshold();
  }
  return isVoltageBelowStopThreshold;
}

bool Battery::isBattVoltageBelowShutdownThreshold()
{
  bool isVoltageBelowShutdownThreshold = false;
  if (0 != m_impl)
  {
    isVoltageBelowShutdownThreshold = m_impl->isBattVoltageBelowShutdownThreshold();
  }
  return isVoltageBelowShutdownThreshold;
}
