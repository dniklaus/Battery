/*
 * Battery.cpp
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#include "Battery.h"
#include "BatteryImpl.h"
//#include <DbgTracePort.h>
//#include <DbgTraceLevel.h>

const float Battery::s_BATT_WARN_THRSHD = 7.5;
const float Battery::s_BATT_STOP_THRSHD = 6.5;
const float Battery::s_BATT_SHUT_THRSHD = 6.1;
const float Battery::s_BATT_HYST        = 0.3;

BatteryAdapter::BatteryAdapter()
: m_battery(0)
//, m_trPort(0)
{ }

void BatteryAdapter::attachBattery(Battery* battery)
{
  m_battery = battery;
  if (0 != m_battery)
  {
    m_trPort = m_battery->trPort();
  }
}

Battery* BatteryAdapter::battery()
{
  return m_battery;
}

void BatteryAdapter::notifyBattVoltageOk()
{
//  if (0 != m_trPort)
//  {
//    TR_PRINTF(m_trPort, DbgTrace_Level::info, "BatteryAdapter::notifyBattVoltageOk()");
//  }
  notifyBattStateAnyChange();
}

void BatteryAdapter::notifyBattVoltageBelowWarnThreshold()
{
//  if (0 != m_trPort)
//  {
//    TR_PRINTF(m_trPort, DbgTrace_Level::info, "BatteryAdapter::notifyBattVoltageBelowWarnThreshold()");
//  }
  notifyBattStateAnyChange();
}

void BatteryAdapter::notifyBattVoltageBelowStopThreshold()
{
//  if (0 != m_trPort)
//  {
//    TR_PRINTF(m_trPort, DbgTrace_Level::info, "BatteryAdapter::notifyBattVoltageBelowStopThreshold()");
//  }
  notifyBattStateAnyChange();
}

void BatteryAdapter::notifyBattVoltageBelowShutdownThreshold()
{
//  if (0 != m_trPort)
//  {
//    TR_PRINTF(m_trPort, DbgTrace_Level::info, "BatteryAdapter::notifyBattVoltageBelowShutdownThreshold()");
//  }
  notifyBattStateAnyChange();
}

void BatteryAdapter::notifyBattStateAnyChange()
{
//  if ((0 != m_trPort) && (0 != m_battery))
//  {
//    TR_PRINTF(m_trPort, DbgTrace_Level::notice, "BatteryAdapter::notifyBattStateAnyChange(), %d.%02dV, %s",
//              static_cast<int>(m_battery->getBatteryVoltage()),
//              static_cast<int>(m_battery->getBatteryVoltage()*100.0)-static_cast<int>(m_battery->getBatteryVoltage())*100,
//              m_battery->isBattVoltageOk() ? "ok" : "/!\\");
//  }
}

float BatteryAdapter::readBattVoltageSenseFactor()
{
  return 2.0;
}

//-----------------------------------------------------------------------------

Battery::Battery(BatteryAdapter* adapter, BatteryThresholdConfig batteryThresholdConfig)
: m_impl(new BatteryImpl(adapter, batteryThresholdConfig))
{
  if (0 != adapter)
  {
    adapter->attachBattery(this);
  }
}

Battery::~Battery()
{
  delete m_impl;
  m_impl = 0;
}

void Battery::attachAdapter(BatteryAdapter* adapter)
{
  if (0 != m_impl)
  {
    m_impl->attachAdapter(adapter);
  }

  if (0 != adapter)
  {
    adapter->attachBattery(this);
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

//DbgTrace_Port* Battery::trPort()
//{
//  DbgTrace_Port* trPort = 0;
//  if (0 != m_impl)
//  {
//    trPort = m_impl->trPort();
//  }
//  return trPort;
//}

