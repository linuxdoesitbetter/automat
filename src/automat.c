#include "automat.h"

//_____________________________________________________________________________
void
triggerStateMachine(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  State ** current_state = &t_state_machine->m_current_state;

  if ( NULL == *current_state ) {

    *current_state = t_state_machine->m_initial_state;

    if ( (*current_state)->m_entry ) {

      Event event = { .m_id = -1, .m_data = NULL, .m_data_size = 0 };
      (*current_state)->m_entry( *current_state, &event );
    }
  }

  Transition ** transition = (*current_state)->m_transitions;

  while ( *transition ) {

    if ( (*transition)->m_event_id == t_event->m_id ) {

      if ( (*current_state)->m_exit ) {

        (*current_state)->m_exit( *current_state, t_event );
      }

      (*current_state) = (*transition)->m_target_state;

      if ( (*current_state)->m_entry ) {

        (*current_state)->m_entry( *current_state, t_event );
      }

      break;
    }

    transition++;
  }

  if ( (*current_state)->m_do ) {

    (*current_state)->m_do( *current_state, t_event );
  }
} //triggerStateMachine
