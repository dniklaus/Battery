/*
 * Battery.h
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#ifndef BATTERY_H_
#define BATTERY_H_

//-----------------------------------------------------------------------------

//class DbgTrace_Port;
class Battery;

class BatteryAdapter
{
private:
#if defined (__arm__) && defined (__SAM3X8E__) // Arduino Due
  const float s_V_ADC_FULLRANGE = 3.3;
  const unsigned int s_N_ADC_FULLRANGE = 1023;
#elif defined (ARDUINO_ARCH_SAMD) && defined (__SAMD21G18A__) // Adafruit Feather M0
  const float s_V_ADC_FULLRANGE = 3.3;
  const unsigned int s_N_ADC_FULLRANGE = 1023;
#elif defined (__AVR__)
  const float s_V_ADC_FULLRANGE = 5.0;
  const unsigned int s_N_ADC_FULLRANGE = 1023;
#elif defined ESP8266
  const float s_V_ADC_FULLRANGE = 1.0;
  const unsigned int s_N_ADC_FULLRANGE = 1023;
#else
  const float s_V_ADC_FULLRANGE = 3.1;
  const unsigned int s_N_ADC_FULLRANGE = 4096;
#endif

  Battery* m_battery;
//  DbgTrace_Port* m_trPort;

public:
  virtual void notifyBattVoltageOk();
  virtual void notifyBattVoltageBelowWarnThreshold();
  virtual void notifyBattVoltageBelowStopThreshold();
  virtual void notifyBattVoltageBelowShutdownThreshold();
  virtual void notifyBattStateAnyChange();
  virtual float readBattVoltageSenseFactor();

  virtual unsigned int readRawBattSenseValue() = 0;

  virtual float getVAdcFullrange()
  {
    return s_V_ADC_FULLRANGE;
  }

  virtual unsigned int getNAdcFullrange()
  {
    return s_N_ADC_FULLRANGE;
  }

  virtual ~BatteryAdapter() { }

  void attachBattery(Battery* battery);
  Battery* battery();

protected:
  BatteryAdapter();

private:  // forbidden default functions
  BatteryAdapter& operator = (const BatteryAdapter& src); // assignment operator
  BatteryAdapter(const BatteryAdapter& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryImpl;

//-----------------------------------------------------------------------------

struct BatteryThresholdConfig
{
  float battWarnThreshd;           /// Battery Voltage Warn Threshold [V]
  float battStopThrshd;            /// Battery Voltage Stop Actors Threshold[V]
  float battShutThrshd;            /// Battery Voltage Shutdown Threshold[V]
  float battHyst;                  /// Battery Voltage Hysteresis around Threshold levels[V]
};

//-----------------------------------------------------------------------------

class Battery
{
public:
  /**
   * Constructor.
   * @param adapter Pointer to a specific BatteryAdapter object, default: 0 (none)
   */
  Battery(BatteryAdapter* adapter = 0, BatteryThresholdConfig batteryThresholdConfig = {Battery::s_BATT_WARN_THRSHD, Battery::s_BATT_STOP_THRSHD, Battery::s_BATT_SHUT_THRSHD, Battery::s_BATT_HYST});

  /**
   * Destructor.
   */
  virtual ~Battery();

  /**
   * Attach a specific BatteryAdapter object.
   * @param adapter Pointer to a specific BatteryAdapter object.
   */
  void attachAdapter(BatteryAdapter* adapter);

  /**
   * Get the pointer to the currently attached specific BatteryAdapter object.
   * @return BatteryAdapter object pointer, might be 0 if none is attached.
   */
  BatteryAdapter* adapter();

  const char* getCurrentStateName();
  const char* getPreviousStateName();

//  DbgTrace_Port* trPort();

  /**
   * Notify Battery Voltage Sense Factor has changed in the Inventory Management Data.
   * The Battery component shall read the new value and adjust the signal conversion accordingly.
   */
  void battVoltageSensFactorChanged();

  /**
   * Read the currently measured Battery Voltage.
   * @return Currently measured Battery Voltage [V].
   */
  float getBatteryVoltage();

  /**
   * Check if the currently measured Battery Voltage is ok.
   * @return true, if voltage is above the warning threshold level, false otherwise.
   */
  bool isBattVoltageOk();

  /**
   * Check if the currently measured Battery Voltage is below the warning threshold level.
   * @return true, if voltage is below the warning threshold level, false otherwise.
   */
  bool isBattVoltageBelowWarnThreshold();

  /**
   * Check if the currently measured Battery Voltage is below the stop threshold level.
   * @return true, if voltage is below the stop threshold level, false otherwise.
   */
  bool isBattVoltageBelowStopThreshold();

  /**
   * Check if the currently measured Battery Voltage is below the shutdown threshold level.
   * @return true, if voltage is below the shutdown threshold level, false otherwise.
   */
  bool isBattVoltageBelowShutdownThreshold();

  /**
   * Evaluate Battery state, execute asynchrounously (detached from the caller)
   */
  void evaluateBatteryStateAsync();

  static const float s_BATT_WARN_THRSHD;            /// default Battery Voltage Warn Threshold [V]
  static const float s_BATT_STOP_THRSHD;            /// default Battery Voltage Stop Actors Threshold [V]
  static const float s_BATT_SHUT_THRSHD;            /// default Battery Voltage Shutdown Threshold [V]
  static const float s_BATT_HYST;                   /// default Battery Voltage Hysteresis around Threshold levels [V]

private:
  BatteryImpl* m_impl;  /// Pointer to the private implementation of the Battery component object.

private: // forbidden default functions
  Battery& operator = (const Battery& src); // assignment operator
  Battery(const Battery& src);              // copy constructor
};

//-----------------------------------------------------------------------------

#endif /* BATTERY_H_ */
