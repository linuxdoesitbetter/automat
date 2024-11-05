#ifndef AUTOMAT_H_AN_20231125
#define AUTOMAT_H_AN_20231125

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//Typ-Deklarationen

typedef enum {
  EVENT_TYPE_BASE = 0,
  EVENT_TYPE_CHANGE,
  EVENT_TYPE_SIGNAL
} EventType;

typedef enum {
  STATE_TYPE_BASE = 0,
  STATE_TYPE_SIMPLE,
  STATE_TYPE_COMPOSITE,
  STATE_TYPE_ENTRY_POINT,
  STATE_TYPE_EXIT_POINT
} StateType;

struct SEvent;
typedef struct SEvent Event;

struct SChangeEvent;
typedef struct SChangeEvent ChangeEvent;

struct SSignalEvent;
typedef struct SSignalEvent SignalEvent;

struct SActivity;
typedef struct SActivity Activity;

struct STransition;
typedef struct STransition Transition;

struct SState;
typedef struct SState State;

struct SSimpleState;
typedef struct SSimpleState SimpleState;

struct SCompositeState;
typedef struct SCompositeState CompositeState;

struct SEntryPoint;
typedef struct SEntryPoint EntryPoint; 

struct SExitPoint;
typedef struct SExitPoint ExitPoint;

struct SStateMachine;
typedef struct SStateMachine StateMachine;

//Typ-Definitionen

typedef void
(*Action)(
    SignalEvent * t_event,
    void *        t_context );

typedef bool
(*Condition)(
    void * t_context );

typedef void
(*Effect)(
    SignalEvent * t_event,
    void *        t_context );

typedef bool
(*Guard)(
    SignalEvent * t_event,
    void *        t_context );

struct SEvent {
  int m_type_id;
};

struct SChangeEvent {
  int       m_type_id;
  Condition m_condition;
};

struct SSignalEvent {
  int    m_type_id;
  int    m_signal_id;
  void * m_data;
  int    m_data_size;
};

struct SActivity {
  Event *  m_event;
  Guard    m_guard;
  Action   m_action_handler;
};

struct STransition {
  Event * m_event;
  State * m_target_state;
  Guard   m_guard;
  Effect  m_effect;
};

struct SState {
  const char * const m_name;
  int                m_type_id;
  StateMachine *     m_parent;
};

struct SSimpleState {
  const char * const m_name;
  int                m_type_id;
  StateMachine *     m_parent;
  Action             m_entry;
  Action             m_do;
  Action             m_exit;
  Activity **        m_activities;
  Transition **      m_transitions;
};

struct SCompositeState {
  const char * const m_name;
  int                m_type_id;
  StateMachine *     m_parent;
  StateMachine **    m_state_machines;
};

struct SEntryPoint {
  const char * const m_name;
  int                m_type_id;
  StateMachine *     m_parent;
  CompositeState *   m_local_target;
  State *            m_target_state;
};

struct SExitPoint {
  const char * const m_name;
  int                m_type_id;
  StateMachine *     m_parent;
  State *            m_target_state;
};

struct SStateMachine {
  const char * const m_name;
  StateMachine *     m_parent;
  void *             m_context;
  State *            m_current_state;
  Transition         m_initial_transition;
};

//Funktions-Deklarationen

void
processStateMachine(
    StateMachine * t_state_machine,
    SignalEvent  * t_event );

#ifdef __cplusplus
}
#endif

#endif //AUTOMAT_H_AN_20231125
