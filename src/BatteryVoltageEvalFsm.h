/*
 * BatteryVoltageEvalFsm.h
 *
 *  Created on: 16.05.2014
 *      Author: niklausd
 */

#ifndef BATTERYVOLTAGEEVALFSM_H_
#define BATTERYVOLTAGEEVALFSM_H_

class BatteryImpl;
class BatteryAdapter;
class BatteryVoltageEvalFsmState;

class BatteryVoltageEvalFsm
{
public:
  BatteryVoltageEvalFsm(BatteryImpl* battImpl);
  virtual ~BatteryVoltageEvalFsm();

  /**
   *
   */
  BatteryVoltageEvalFsmState* state();

  /**
   *
   */
  BatteryVoltageEvalFsmState* previousState();

  /**
   *
   */
  BatteryAdapter* adapter();

  /**
   *
   */
  void changeState(BatteryVoltageEvalFsmState* state);

  /**
   *
   */
  void evaluateStatus();

  bool isBattVoltageOk();
  bool isBattVoltageBelowWarnThreshold();
  bool isBattVoltageBelowStopThreshold();
  bool isBattVoltageBelowShutdownThreshold();

private:
  friend class BatteryVoltageEvalFsmState_BattOk;
  friend class BatteryVoltageEvalFsmState_BattNotOk;
  friend class BatteryVoltageEvalFsmState_BattVoltageBelowWarn;
  friend class BatteryVoltageEvalFsmState_BattVoltageBelowStop;
  friend class BatteryVoltageEvalFsmState_BattVoltageBelowShutdown;
  bool isGuardWarn();
  bool isGuardStop();
  bool isGuardShut();
  bool isGuardWarnPlusHyst();
  bool isGuardStopPlusHyst();
  bool isGuardShutPlusHyst();

private:
  BatteryImpl* m_battImpl;
  BatteryAdapter* m_adapter;
  BatteryVoltageEvalFsmState* m_state;
  BatteryVoltageEvalFsmState* m_previousState;

private: // forbidden default functions
  BatteryVoltageEvalFsm& operator = (const BatteryVoltageEvalFsm& src); // assignment operator
  BatteryVoltageEvalFsm(const BatteryVoltageEvalFsm& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryVoltageEvalFsmState
{
protected:
  BatteryVoltageEvalFsmState() { }

public:
  virtual ~BatteryVoltageEvalFsmState() { }

  virtual void evaluateState(BatteryVoltageEvalFsm* fsm) { }

  virtual void entry(BatteryVoltageEvalFsm* fsm) { }

//  virtual void evalExit(BatteryVoltageEvalFsm* fsm) { }

  virtual const char* toString() = 0;

private: // forbidden default functions
  BatteryVoltageEvalFsmState& operator = (const BatteryVoltageEvalFsmState& src); // assignment operator
  BatteryVoltageEvalFsmState(const BatteryVoltageEvalFsmState& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryVoltageEvalFsmState_BattOk : public BatteryVoltageEvalFsmState
{
private:
  BatteryVoltageEvalFsmState_BattOk() { }

public:
  static BatteryVoltageEvalFsmState* Instance();

  virtual ~BatteryVoltageEvalFsmState_BattOk() { }

  virtual void evaluateState(BatteryVoltageEvalFsm* fsm);

  virtual void entry(BatteryVoltageEvalFsm* fsm);

  virtual const char* toString();

private:
  static BatteryVoltageEvalFsmState* s_instance;

private: // forbidden default functions
  BatteryVoltageEvalFsmState_BattOk& operator = (const BatteryVoltageEvalFsmState_BattOk& src); // assignment operator
  BatteryVoltageEvalFsmState_BattOk(const BatteryVoltageEvalFsmState_BattOk& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryVoltageEvalFsmState_BattVoltageBelowWarn : BatteryVoltageEvalFsmState
{
private:
  BatteryVoltageEvalFsmState_BattVoltageBelowWarn() { }

public:
  static BatteryVoltageEvalFsmState* Instance();

  virtual ~BatteryVoltageEvalFsmState_BattVoltageBelowWarn() { }

  virtual void evaluateState(BatteryVoltageEvalFsm* fsm);

  virtual void entry(BatteryVoltageEvalFsm* fsm);

  virtual const char* toString();

private:
  static BatteryVoltageEvalFsmState* s_instance;

private: // forbidden default functions
  BatteryVoltageEvalFsmState_BattVoltageBelowWarn& operator = (const BatteryVoltageEvalFsmState_BattVoltageBelowWarn& src); // assignment operator
  BatteryVoltageEvalFsmState_BattVoltageBelowWarn(const BatteryVoltageEvalFsmState_BattVoltageBelowWarn& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryVoltageEvalFsmState_BattVoltageBelowStop : BatteryVoltageEvalFsmState
{
private:
  BatteryVoltageEvalFsmState_BattVoltageBelowStop() { }

public:
  static BatteryVoltageEvalFsmState* Instance();

  virtual ~BatteryVoltageEvalFsmState_BattVoltageBelowStop() { }

  virtual void evaluateState(BatteryVoltageEvalFsm* fsm);

  virtual void entry(BatteryVoltageEvalFsm* fsm);

  virtual const char* toString();

private:
  static BatteryVoltageEvalFsmState* s_instance;

private: // forbidden default functions
  BatteryVoltageEvalFsmState_BattVoltageBelowStop& operator = (const BatteryVoltageEvalFsmState_BattVoltageBelowStop& src); // assignment operator
  BatteryVoltageEvalFsmState_BattVoltageBelowStop(const BatteryVoltageEvalFsmState_BattVoltageBelowStop& src);              // copy constructor
};

//-----------------------------------------------------------------------------

class BatteryVoltageEvalFsmState_BattVoltageBelowShutdown : BatteryVoltageEvalFsmState
{
private:
  BatteryVoltageEvalFsmState_BattVoltageBelowShutdown() { }

public:
  static BatteryVoltageEvalFsmState* Instance();

  virtual ~BatteryVoltageEvalFsmState_BattVoltageBelowShutdown() { }

  virtual void evaluateState(BatteryVoltageEvalFsm* fsm);

  virtual void entry(BatteryVoltageEvalFsm* fsm);

  virtual const char* toString();

private:
  static BatteryVoltageEvalFsmState* s_instance;

private: // forbidden default functions
  BatteryVoltageEvalFsmState_BattVoltageBelowShutdown& operator = (const BatteryVoltageEvalFsmState_BattVoltageBelowShutdown& src); // assignment operator
  BatteryVoltageEvalFsmState_BattVoltageBelowShutdown(const BatteryVoltageEvalFsmState_BattVoltageBelowShutdown& src);              // copy constructor
};

//-----------------------------------------------------------------------------

#endif /* BATTERYVOLTAGEEVALFSM_H_ */
