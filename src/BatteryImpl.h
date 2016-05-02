/*
 * BatteryImpl.h
 *
 *  Created on: 30.04.2014
 *      Author: niklausd
 */

#ifndef BATTERYIMPL_H_
#define BATTERYIMPL_H_

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
  BatteryImpl(BatteryAdapter* adapter);

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
   *
   */
  void evaluateStatus();

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

private:
  BatteryAdapter* m_adapter;  /// Pointer to the currently attached specific BatteryAdapter object
  BatteryVoltageEvalFsm* m_evalFsm;
  Timer* m_startupTimer;
  Timer* m_pollTimer;
  float m_batteryVoltage;
  float m_battVoltageSenseFactor;

  static const unsigned int s_DEFAULT_STARTUP_TIME; /// [ms]
  static const unsigned int s_DEFAULT_POLL_TIME;    /// [ms]

  static const unsigned int s_V_ADC_FULLRANGE;      /// [V]
  static const unsigned int s_N_ADC_FULLRANGE;      /// [1]

public:
  static const float s_BATT_WARN_THRSHD;            /// [V]
  static const float s_BATT_STOP_THRSHD;            /// [V]
  static const float s_BATT_SHUT_THRSHD;            /// [V]
  static const float s_BATT_HYST;                   /// [V]


private: // forbidden default functions
  BatteryImpl& operator = (const BatteryImpl& src); // assignment operator
  BatteryImpl(const BatteryImpl& src);              // copy constructor
};

#endif /* BATTERYIMPL_H_ */
