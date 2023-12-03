#include "automat.h"
#include <stdio.h>

enum EventId {
  EV_NONE,
  EV_SWITCH,
  EV_COUNT
};

typedef struct STestMachineContext {
  int m_counter;
} TestMachineContext;

void
effectStart(
    StateMachine * t_state_machine,
    Event        * t_event );

void
entryOff(
    StateMachine * t_state_machine,
    Event        * t_event );

void
entryOn(
    StateMachine * t_state_machine,
    Event        * t_event );

void
doOnOff(
    StateMachine * t_state_machine,
    Event        * t_event );

void
exitOnOff(
    StateMachine * t_state_machine,
    Event        * t_event );

void
actionOffCount(
    StateMachine * t_state_machine,
    Event        * t_event );

bool
actionOffCountGuard(
    StateMachine * t_state_machine,
    Event        * t_event );

SimpleState state_on;
SimpleState state_off;

TestMachineContext tm_context = {
  .m_counter = 0
};

Action action_off_count = {
  .m_event_id       = EV_COUNT,
  .m_guard          = actionOffCountGuard,
  .m_action_handler = actionOffCount,
};

Transition transition_off_on = {
  .m_event_id     = EV_SWITCH,
  .m_target_state = (State*)&state_on,
  .m_guard        = NULL,
  .m_effect       = NULL
};

Transition transition_on_off = {
  .m_event_id     = EV_SWITCH,
  .m_target_state = (State*)&state_off,
  .m_guard        = NULL,
  .m_effect       = NULL
};

SimpleState state_off = {
  .m_name        = "State Off",
  .m_type_id     = STATE_TYPE_SIMPLE,
  .m_entry       = entryOff,
  .m_do          = doOnOff,
  .m_exit        = exitOnOff,
  .m_actions     = (Action*[]){
      &action_off_count,
      NULL
  },
  .m_transitions = (Transition*[]){
      &transition_off_on,
      NULL }
};

SimpleState state_on = {
  .m_name        = "State On",
  .m_type_id     = STATE_TYPE_SIMPLE,
  .m_entry       = entryOn,
  .m_do          = doOnOff,
  .m_exit        = exitOnOff,
  .m_actions     = NULL,
  .m_transitions = (Transition*[]){
      &transition_on_off,
      NULL }
};

StateMachine state_machine = {
  .m_name               = "Test Machine",
  .m_accepted           = false,
  .m_context            = &tm_context,
  .m_current_state      = NULL,
  .m_initial_transition = {
      .m_event_id       = -1,
      .m_target_state   = (State*)&state_off,
      .m_guard          = NULL,
      .m_effect         = effectStart },
  .m_states             = (State*[]) {
      (State*)&state_off,
      (State*)&state_on,
      NULL
  }
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
  Event event_count  = { .m_id = EV_COUNT, .m_data = NULL, .m_data_size = 0 };

  processStateMachine( &state_machine, &event_none );
  processStateMachine( &state_machine, &event_count );
  processStateMachine( &state_machine, &event_switch );
  processStateMachine( &state_machine, &event_none );
  processStateMachine( &state_machine, &event_count );
  processStateMachine( &state_machine, &event_none );
  processStateMachine( &state_machine, &event_switch );
  processStateMachine( &state_machine, &event_count );
  processStateMachine( &state_machine, &event_none );
  return result;
} //main

//_____________________________________________________________________________
void
entryOff(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  printf(
      "%-10s (Event: %2d) entry / switch off\n",
      t_state_machine->m_current_state->m_name,
      t_event->m_id );
} //entryOff

//_____________________________________________________________________________
void
entryOn(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  printf(
      "%-10s (Event: %2d) entry / switch on\n",
      t_state_machine->m_current_state->m_name,
      t_event->m_id );
} //entryOn

//_____________________________________________________________________________
void
doOnOff(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  printf(
      "%-10s (Event: %2d) do    /\n",
      t_state_machine->m_current_state->m_name,
      t_event->m_id );
} //doOnOff

//_____________________________________________________________________________
void
exitOnOff(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  printf(
      "%-10s (Event: %2d) exit  / \n",
      t_state_machine->m_current_state->m_name,
      t_event->m_id );
} //exitOnOff

//_____________________________________________________________________________
void
actionOffCount(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  (void)t_event;
  TestMachineContext * context =
      (TestMachineContext *)t_state_machine->m_context;

  context->m_counter++;
  printf(
      "%-10s (Event: %2d) action_off_count / tm_context.m_counter++ == %d\n",
      t_state_machine->m_current_state->m_name,
      t_event->m_id,
      context->m_counter );
} //actionOffCount

//_____________________________________________________________________________
bool
actionOffCountGuard(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  (void)t_state_machine;
  (void)t_event;

  return true;
} //actionOffCountGurad

//_____________________________________________________________________________
void
effectStart(
    StateMachine * t_state_machine,
    Event        * t_event ) {

  printf(
      "Starte die StateMachine \"%s\" mit dem Event Nr. %d.\n",
      t_state_machine->m_name,
      t_event->m_id );
} //effectStart
