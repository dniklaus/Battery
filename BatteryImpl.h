/*
 * BatteryImpl.h
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#ifndef BATTERYIMPL_H_
#define BATTERYIMPL_H_

#include "Battery.h"

class Timer;
class BatteryAdapter;
class BatteryVoltageEvalFsm;

class BatteryImpl
{
public:
  /**
   * Constructor.
   * @param adapter Pointer to a specific BatteryAdapter object.
   */
  BatteryImpl(BatteryAdapter* adapter, BatteryThresholdConfig batteryThresholdConfig);

  /**
   * Destructor.
   */
  virtual ~BatteryImpl();

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

  /**
   * Notify application startup (after startup timer expired).
   */
  void startup();

  /**
   * Read battery voltage and evaluate battery status.
   */
  void evaluateStatus();

  /**
   * Read battery voltage and evaluate battery status, execute asynchronously (detached from the caller)
   */
  void evaluateStatusAsync();

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

  const char* getCurrentStateName();
  const char* getPreviousStateName();

  float battWarnThreshd();           /// Battery Voltage Warn Threshold [V]
  float battStopThrshd();            /// Battery Voltage Stop Actors Threshold[V]
  float battShutThrshd();            /// Battery Voltage Shutdown Threshold[V]
  float battHyst();                  /// Battery Voltage Hysteresis around Threshold levels[V]

private:
  BatteryAdapter* m_adapter;  /// Pointer to the currently attached specific BatteryAdapter object
  BatteryVoltageEvalFsm* m_evalFsm;
  Timer* m_startupTimer;
  Timer* m_pollTimer;
  Timer* m_evalStatusTimer;

  float m_batteryVoltage;
  float m_battVoltageSenseFactor;



  float m_battWarnThreshd;           /// Battery Voltage Warn Threshold [V]
  float m_battStopThrshd;            /// Battery Voltage Stop Actors Threshold [V]
  float m_battShutThrshd;            /// Battery Voltage Shutdown Threshold [V]
  float m_battHyst;                  /// Battery Voltage Hysteresis around Threshold levels [V]


  static const unsigned int s_DEFAULT_STARTUP_TIME;           /// startup timer time[ms]
  static const unsigned int s_DEFAULT_POLL_TIME;              /// status poll interval [ms]
  static const unsigned int s_DEFAULT_ASYNC_STATUS_EVAL_TIME; /// asynchronous status eval time [ms]

private: // forbidden default functions
  BatteryImpl& operator = (const BatteryImpl& src); // assignment operator
  BatteryImpl(const BatteryImpl& src);              // copy constructor
};

#endif /* BATTERYIMPL_H_ */
