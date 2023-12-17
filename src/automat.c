#include "automat.h"

static inline bool
eventTriggers(
    StateMachine * t_state_machine,
    Event        * t_defined,
    Guard          t_guard,
    SignalEvent  * t_given );

static inline void
processCompositeState(
    CompositeState * t_current_state,
    SignalEvent    * t_event );

static inline void
processSimpleState(
    StateMachine * t_state_machine,
    SimpleState  * t_current_state,
    SignalEvent  * t_event );

static inline void
processTransitions(
    StateMachine * t_state_machine,
    SimpleState  * t_current_state,
    SignalEvent  * t_event );

static inline void
processActions(
    StateMachine * t_state_machine,
    SimpleState  * t_current_state,
    SignalEvent  * t_event );

//_____________________________________________________________________________
static inline bool
eventTriggers(
    StateMachine * t_state_machine,
    Event        * t_defined,
    Guard          t_guard,
    SignalEvent  * t_given ) {

  bool triggers      = false;
  bool event_matches = false;

  switch ( t_defined->m_type_id ) {
  case EVENT_TYPE_SIGNAL:
    if ( ((SignalEvent*)t_defined)->m_signal_id == t_given->m_signal_id ) {

      event_matches = true;
    }
    break;
  case EVENT_TYPE_CHANGE:
    if ( ((ChangeEvent*)t_given)->m_condition( t_state_machine) ) {

      event_matches = true;
    }
    break;
  default:
    break;
  }

  if ( event_matches ) {

    if ( (NULL == t_guard) || t_guard( t_state_machine, t_given ) ) {

      triggers = true;
    }
  }

  return triggers;
} //eventTriggers

//_____________________________________________________________________________
void
processStateMachine(
    StateMachine * t_state_machine,
    SignalEvent  * t_event ) {

  State ** current_state = &t_state_machine->m_current_state;

  //Start der StateMachine behandeln.
  if ( NULL == *current_state ) {

    *current_state = t_state_machine->m_initial_transition.m_target_state;

    if ( t_state_machine->m_initial_transition.m_effect ) {

      t_state_machine->m_initial_transition.m_effect(
          t_state_machine,
          t_event );
    }

    switch ( (*current_state)->m_type_id ) {
    case STATE_TYPE_SIMPLE:
      if ( ((SimpleState*)*current_state)->m_entry ) {

        ((SimpleState*)*current_state)->m_entry(
            t_state_machine,
            t_event );
      }
      break;
    case STATE_TYPE_COMPOSITE:
      break;
    default:
      break;
    }
  }

  switch ( (*current_state)->m_type_id ) {
  case STATE_TYPE_SIMPLE:
    processSimpleState(
        t_state_machine,
        (SimpleState*)(*current_state),
        t_event );
    break;
  case STATE_TYPE_COMPOSITE:
    processCompositeState(
        (CompositeState*)(*current_state),
        t_event );
    break;
  default:
    break;
  }
} //triggerStateMachine

//_____________________________________________________________________________
static inline void
processTransitions(
    StateMachine * t_state_machine,
    SimpleState  * t_current_state,
    SignalEvent  * t_event ) {

  Transition ** transition = t_current_state->m_transitions;

  if ( transition ) {

    while ( *transition ) {

      bool triggers = eventTriggers(
          t_state_machine,
          (*transition)->m_event,
          (*transition)->m_guard,
          t_event );

      if ( triggers ) {

        //Exit-Aktion des bishherigen Zustands aufrufen.
        if ( t_current_state->m_exit ) {

          t_current_state->m_exit( t_state_machine, t_event );
        }

        //Effekt der Transition ausführen.
        if ( (*transition)->m_effect ) {

          (*transition)->m_effect( t_state_machine, t_event );
        }

        //Zustand wechseln.
        t_state_machine->m_current_state = (*transition)->m_target_state;

        switch ( t_state_machine->m_current_state->m_type_id ) {
        case STATE_TYPE_SIMPLE:
          //Entry-Aktion des neuen Zustands aufrufen.
          if ( t_current_state->m_entry ) {

            ((SimpleState*)t_current_state)->m_entry(
                t_state_machine,
                t_event );
          }
          break;
        case STATE_TYPE_COMPOSITE:
          break;
        case STATE_TYPE_EXIT_POINT:
          break;
        default:
          break;
        }
        break;
      }

      transition++;
    }
  }
} //processTransitions

//_____________________________________________________________________________
static inline void
processActions(
    StateMachine * t_state_machine,
    SimpleState  * t_current_state,
    SignalEvent  * t_event ) {

  Action ** action = t_current_state->m_actions;

  if ( action ) {

    while ( *action ) {

      if ( (*action)->m_action_handler ) {

        bool triggers = eventTriggers(
            t_state_machine,
            (*action)->m_event,
            (*action)->m_guard,
            t_event );

        if ( triggers ) {

          (*action)->m_action_handler( t_state_machine, t_event );
        }
      }

      action++;
    }
  }
} //processActions

//_____________________________________________________________________________
static inline void
processCompositeState(
    CompositeState * t_current_state,
    SignalEvent    * t_event ) {

  StateMachine ** state_machine = t_current_state->m_state_machines;

  if ( state_machine ) {

    while ( *state_machine ) {

      processStateMachine( *state_machine, t_event );
    }
  }
} //processCompositeState

//_____________________________________________________________________________
static inline void
processSimpleState(
    StateMachine * t_state_machine,
    SimpleState  * t_current_state,
    SignalEvent  * t_event ) {

  //Do-Aktion des aktuellen Zustands ausführen.
  if ( t_current_state->m_do ) {

    t_current_state->m_do( t_state_machine, t_event );
  }

  processTransitions( t_state_machine, t_current_state, t_event );
  processActions( t_state_machine, t_current_state, t_event );
} //processSimpleState
