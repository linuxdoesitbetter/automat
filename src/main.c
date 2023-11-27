#include "automat.h"
#include <stdio.h>

enum EventId {
  EV_NONE,
  EV_SWITCH
};

void
entryOff(
    State * t_state,
    Event * t_event );

void
entryOn(
    State * t_state,
    Event * t_event );

void
doOnOff(
    State * t_state,
    Event * t_event );

void
exitOnOff(
    State * t_state,
    Event * t_event );

State state_on;
State state_off;

Transition transition_off_on = {
  .m_event_id     = EV_SWITCH,
  .m_target_state = &state_on
};

Transition transition_on_off = {
  .m_event_id     = EV_SWITCH,
  .m_target_state = &state_off
};

State state_off = {
  .m_entry       = entryOff,
  .m_do          = doOnOff,
  .m_exit        = exitOnOff,
  .m_transitions = (Transition*[]){
      &transition_off_on,
      NULL },
  .m_name        = "State Off"
};

State state_on = {
  .m_entry       = entryOn,
  .m_do          = doOnOff,
  .m_exit        = exitOnOff,
  .m_transitions = (Transition*[]){
      &transition_on_off,
      NULL },
  .m_name        = "State On"
};

StateMachine state_machine = {
  .m_accepted      = false,
  .m_initial_state = &state_off,
  .m_current_state = NULL,
  .m_states        = (State*[]) {
      &state_off,
      &state_on,
      NULL }
};

//_____________________________________________________________________________
int
main(
    int     t_argc,
    char ** t_argv ) {

  (void) t_argc;
  (void) t_argv;

  int result = 0;

  Event event_none   = { .m_id = EV_NONE, .m_data = NULL, .m_data_size = 0 };
  Event event_switch = { .m_id = EV_SWITCH, .m_data = NULL, .m_data_size = 0 };

  triggerStateMachine( &state_machine, &event_none );
  triggerStateMachine( &state_machine, &event_switch );
  triggerStateMachine( &state_machine, &event_none );
  triggerStateMachine( &state_machine, &event_none );
  triggerStateMachine( &state_machine, &event_switch );

  return result;
} //main

//_____________________________________________________________________________
void
entryOff(
    State * t_state,
    Event * t_event ) {

  printf(
      "%-10s (Event: %2d) entry / switch off\n",
      t_state->m_name,
      t_event->m_id );
} //entryOff

//_____________________________________________________________________________
void
entryOn(
    State * t_state,
    Event * t_event ) {

  printf(
      "%-10s (Event: %2d) entry / switch on\n",
      t_state->m_name,
      t_event->m_id );
} //entryOn

//_____________________________________________________________________________
void
doOnOff(
    State * t_state,
    Event * t_event ) {

  printf(
      "%-10s (Event: %2d) do    /\n",
      t_state->m_name,
      t_event->m_id );
} //doOnOff

//_____________________________________________________________________________
void
exitOnOff(
    State * t_state,
    Event * t_event ) {

  printf(
      "%-10s (Event: %2d) exit  / \n",
      t_state->m_name,
      t_event->m_id );
} //exitOnOff
