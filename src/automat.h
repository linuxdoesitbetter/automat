#ifndef AUTOMAT_H_AN_20231125
#define AUTOMAT_H_AN_20231125

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//Typ-Deklarationen

struct SEvent;
typedef struct SEvent Event;

struct SAction;
typedef struct SAction Action;

struct STransition;
typedef struct STransition Transition;

struct SState;
typedef struct SState State;

struct SStateMachine;
typedef struct SStateMachine StateMachine;

//Typ-Definitionen

typedef void (*ActionHandler)( StateMachine * t_state_machine, Event * t_event );
typedef void (*Effect)( StateMachine * t_state_machine, Event * t_event );
typedef bool (*Guard)( StateMachine * t_state_machine, Event * t_event );


struct SEvent {
  int    m_id;
  void * m_data;
  int    m_data_size;
};

struct SAction {
  int           m_event_id;
  Guard         m_guard;
  ActionHandler m_action_handler;
};

struct STransition {
  int      m_event_id;
  State  * m_target_state;
  Guard    m_guard;
  Effect   m_effect;
};

struct SState {
  const char * const m_name;
  ActionHandler      m_entry;
  ActionHandler      m_do;
  ActionHandler      m_exit;
  Action **          m_actions;
  Transition **      m_transitions;
};

struct SStateMachine {
  const char * const m_name;
  bool               m_accepted;
  void *             m_context;
  State *            m_current_state;
  Transition         m_initial_transition;
  State **           m_states;
};

//Funktions-Deklarationen

void
processStateMachine(
    StateMachine * t_state_machine,
    Event        * t_event );

#endif //AUTOMAT_H_AN_20231125
