#include "automat.h"

static inline void
processActions(
    SimpleState * t_current_state,
    SignalEvent * t_event,
    void *        t_context );

static inline bool
eventTriggers(
    Event *        t_defined,
    Guard          t_guard,
    SignalEvent *  t_given,
    void        *  t_context );

static inline void
processCompositeState(
    CompositeState * t_current_state,
    SignalEvent *    t_event );

static inline void
processExitPoint(
    ExitPoint *   t_exit_point,
    SignalEvent * t_event );

static inline void
processSimpleState(
    SimpleState *  t_current_state,
    SignalEvent *  t_event );

static inline bool
processTransition(
    SimpleState * t_current_state,
    SignalEvent * t_event,
    Transition *  t_transition );

static inline void
processTransitions(
    SimpleState * t_current_state,
    SignalEvent * t_event );

//_____________________________________________________________________________
static inline bool
eventTriggers(
    Event *        t_defined,
    Guard          t_guard,
    SignalEvent *  t_given,
    void *         t_context ) {

  bool triggers      = false;
  bool event_matches = false;

  if ( t_defined ) {

    switch ( t_defined->m_type_id ) {
    case EVENT_TYPE_SIGNAL:
      if ( ((SignalEvent*)t_defined)->m_signal_id == t_given->m_signal_id ) {

        event_matches = true;
      }
      break;
    case EVENT_TYPE_CHANGE:
      if ( ((ChangeEvent*)t_defined)->m_condition( t_context ) ) {

        event_matches = true;
      }
      break;
    default:
      break;
    }

    if ( event_matches ) {

      if ( (NULL == t_guard) || t_guard( t_given, t_context ) ) {

        triggers = true;
      }
    }
  }
  else {

    triggers = true;
  }

  return triggers;
} //eventTriggers

//_____________________________________________________________________________
void
processStateMachine(
    StateMachine * t_state_machine,
    SignalEvent *  t_event ) {

  State ** current_state = &t_state_machine->m_current_state;

  //Start der StateMachine behandeln.
  if ( NULL == *current_state ) {

    *current_state = t_state_machine->m_initial_transition.m_target_state;

    if ( t_state_machine->m_initial_transition.m_effect ) {

      t_state_machine->m_initial_transition.m_effect(
          t_event,
          t_state_machine->m_context );
    }

    switch ( (*current_state)->m_type_id ) {
    case STATE_TYPE_SIMPLE:
      if ( ((SimpleState*)*current_state)->m_entry ) {

        ((SimpleState*)*current_state)->m_entry(
            t_event,
            t_state_machine->m_context );
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
static inline bool
processTransition(
    SimpleState * t_current_state,
    SignalEvent * t_event,
    Transition *  t_transition ) {

  StateMachine * state_machine = t_current_state->m_parent;
  void *         context       = state_machine->m_context;

  bool triggers = eventTriggers(
      t_transition->m_event,
      t_transition->m_guard,
      t_event,
      context );

  if ( triggers ) {

    //Exit-Aktion des bishherigen Zustands aufrufen.
    if ( t_current_state->m_exit ) {

      t_current_state->m_exit( t_event, context );
    }

    //Effekt der Transition ausführen.
    if ( t_transition->m_effect ) {

      t_transition->m_effect( t_event, context );
    }

    //Zustand wechseln.
    state_machine->m_current_state = t_transition->m_target_state;

    switch ( state_machine->m_current_state->m_type_id ) {
    case STATE_TYPE_SIMPLE:      
      //Entry-Aktion des neuen Zustands aufrufen.
      if ( ((SimpleState*)state_machine->m_current_state)->m_entry ) {

        ((SimpleState*)state_machine->m_current_state)->m_entry(
            t_event,
            context );
      }
      break;
    case STATE_TYPE_COMPOSITE:
      break;
    case STATE_TYPE_EXIT_POINT:
      processExitPoint(
          (ExitPoint*)state_machine->m_current_state,
          t_event );
      break;
    default:
      break;
    }
  }

  return triggers;
} //processTransition

//_____________________________________________________________________________
static inline void
processTransitions(
    SimpleState *  t_current_state,
    SignalEvent *  t_event ) {

  Transition ** transition = t_current_state->m_transitions;

  if ( transition ) {

    while ( *transition ) {

      if ( processTransition(
          t_current_state,
          t_event,
          *transition ) ) {

        break;
      }
      transition++;
    }
  }
} //processTransitions

//_____________________________________________________________________________
static inline void
processActions(
    SimpleState * t_current_state,
    SignalEvent * t_event,
    void *        t_context ) {

  Action ** action = t_current_state->m_actions;

  if ( action ) {

    while ( *action ) {

      if ( (*action)->m_action_handler ) {

        bool triggers = eventTriggers(
            (*action)->m_event,
            (*action)->m_guard,
            t_event,
            t_context );

        if ( triggers ) {

          (*action)->m_action_handler( t_event, t_context );
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
    SignalEvent *    t_event ) {

  StateMachine ** state_machine = t_current_state->m_state_machines;

  if ( state_machine ) {

    while ( *state_machine ) {

      processStateMachine( *state_machine, t_event );
      state_machine++;
    }
  }
} //processCompositeState

//processExitPoint_____________________________________________________________
static inline void
processExitPoint(
    ExitPoint *   t_exit_point,
    SignalEvent * t_event ) {

  StateMachine * super_sm = t_exit_point->m_target_state->m_parent;

  //Substatemachine verlassen
  t_exit_point->m_parent->m_current_state = NULL;

  //In den Zielzustand der Superstatemachine wechseln.
  super_sm->m_current_state = t_exit_point->m_target_state;

  switch ( super_sm->m_current_state->m_type_id ) {
  case STATE_TYPE_SIMPLE:
    if ( ((SimpleState*)super_sm->m_current_state)->m_entry ) {

      ((SimpleState*)super_sm->m_current_state)->m_entry(
          t_event,
          super_sm->m_context );
    }
    break;
  case STATE_TYPE_COMPOSITE:
    break;
  default:
    break;
  }
} //processExitPoint

//_____________________________________________________________________________
static inline void
processSimpleState(
    SimpleState *  t_current_state,
    SignalEvent *  t_event ) {

  void * context = t_current_state->m_parent->m_context;

  //Do-Aktion des aktuellen Zustands ausführen.
  if ( t_current_state->m_do ) {

    t_current_state->m_do( t_event, context );
  }

  processActions( t_current_state, t_event, context );
  processTransitions( t_current_state, t_event );  
} //processSimpleState
