/*
 * Battery.h
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#ifndef BATTERY_H_
#define BATTERY_H_

//-----------------------------------------------------------------------------

class BatteryAdapter
{
public:
  virtual void notifyBattVoltageOk()                      { }
  virtual void notifyBattVoltageBelowWarnThreshold()      { }
  virtual void notifyBattVoltageBelowStopThreshold()      { }
  virtual void notifyBattVoltageBelowShutdownThreshold()  { }
  virtual float readBattVoltageSenseFactor()              = 0;
  virtual unsigned int readRawBattSenseValue()            = 0;

  virtual ~BatteryAdapter() { }

protected:
  BatteryAdapter() { }

private:  // forbidden default functions
  BatteryAdapter& operator = (const BatteryAdapter& src); // assignment operator
  BatteryAdapter(const BatteryAdapter& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryImpl;

//-----------------------------------------------------------------------------

class Battery
{
public:
  /**
   * Constructor.
   * @param adapter Pointer to a specific BatteryAdapter object, default: 0 (none)
   */
  Battery(BatteryAdapter* adapter = 0);

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

private:
  BatteryImpl* m_impl;  /// Pointer to the private implementation of the Battery component object.

private: // forbidden default functions
  Battery& operator = (const Battery& src); // assignment operator
  Battery(const Battery& src);              // copy constructor
};

//-----------------------------------------------------------------------------

#endif /* BATTERY_H_ */
