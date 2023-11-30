#include "automat.h"

static inline void
processTransitions(
    StateMachine *  t_state_machine,
    State        ** t_current_state,
    Event        *  t_event );

static inline void
processActions(
    StateMachine *  t_state_machine,
    State        ** t_current_state,
    Event        *  t_event );

//_____________________________________________________________________________
void
processStateMachine(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  State ** current_state = &t_state_machine->m_current_state;

  //Start der StateMachine behandeln.
  if ( NULL == *current_state ) {

    *current_state = t_state_machine->m_initial_transition.m_target_state;

    if ( t_state_machine->m_initial_transition.m_effect ) {

      t_state_machine->m_initial_transition.m_effect(
          t_state_machine,
          t_event );
    }

    if ( (*current_state)->m_entry ) {

      (*current_state)->m_entry( t_state_machine, t_event );
    }
  }

  //Do-Aktion des aktuellen Zustands ausführen.
  if ( (*current_state)->m_do ) {

    (*current_state)->m_do( t_state_machine, t_event );
  }

  processTransitions( t_state_machine, current_state, t_event );
  processActions( t_state_machine, current_state, t_event );

} //triggerStateMachine

//_____________________________________________________________________________
static inline void
processTransitions(
    StateMachine *  t_state_machine,
    State        ** t_current_state,
    Event        *  t_event ) {

  Transition ** transition = (*t_current_state)->m_transitions;

  if ( transition ) {

    while ( *transition ) {

      if ( (*transition)->m_event_id == t_event->m_id ) {

        if (    ( NULL ==  (*transition)->m_guard )
             || (*transition)->m_guard( t_state_machine, t_event ) ) {

          //Exit-Aktion des bishherigen Zustands aufrufen.
          if ( (*t_current_state)->m_exit ) {

            (*t_current_state)->m_exit( t_state_machine, t_event );
          }

          //Effekt der Transition ausführen.
          if ( (*transition)->m_effect ) {

            (*transition)->m_effect( t_state_machine, t_event );
          }

          //Zustand wechseln.
          (*t_current_state) = (*transition)->m_target_state;

          //Entry-Aktion des neuen Zustands aufrufen.
          if ( (*t_current_state)->m_entry ) {

            (*t_current_state)->m_entry( t_state_machine, t_event );
          }

          break;
        }
      }

      transition++;
    }
  }
} //processTransitions

//_____________________________________________________________________________
static inline void
processActions(
    StateMachine *  t_state_machine,
    State        ** t_current_state,
    Event        *  t_event ) {

  Action ** action = (*t_current_state)->m_actions;

  if ( action ) {

    while ( *action ) {

      if ( (*action)->m_event_id == t_event->m_id ) {

        if ( (*action)->m_action_handler ) {

          if (    ( NULL ==  (*action)->m_guard )
               || (*action)->m_guard( t_state_machine, t_event ) ) {

            (*action)->m_action_handler( t_state_machine, t_event );
          }
        }
      }

      action++;
    }
  }
} //processActions
