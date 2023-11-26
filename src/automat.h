#ifndef AUTOMAT_H_AN_20231125
#define AUTOMAT_H_AN_20231125

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//Typ-Deklarationen

struct SEvent;
typedef struct SEvent Event;

struct STransition;
typedef struct STransition Transition;

struct SState;
typedef struct SState State;

struct SStateMachine;
typedef struct SStateMachine StateMachine;

//Typ-Definitionen

typedef void (*Action)( State * t_state, Event * t_event );
typedef bool (*Guard)( State * t_state, Event * t_event );

struct SEvent {
  int    m_id;
  void * m_data;
  int    m_data_size;
};

struct STransition {
  int     m_event_id;
  State * m_target_state;
};

struct SState {
  Action        m_entry;
  Action        m_do;
  Action        m_exit;
  Transition ** m_transitions;
  const char  * m_name;
};

struct SStateMachine {
  bool           m_accepted;
  State  * const m_initial_state;
  State  *       m_current_state;
  State **       m_states;
};

//Funktions-Deklarationen

void
triggerStateMachine(
    StateMachine * t_state_machine,
    Event        * t_event );

#endif //AUTOMAT_H_AN_20231125
