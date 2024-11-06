#include "automat.h"
#include <stdio.h>

#define CONTEXT ((TestMachineContext*)(t_context))

enum EventId {
  EV_NONE,
  EV_SWITCH,
  EV_COUNT
};

extern StateMachine state_machine;

SignalEvent switch_event = {
  .m_type_id   = EVENT_TYPE_SIGNAL,
  .m_signal_id = EV_SWITCH,
  .m_data      = NULL,
  .m_data_size = 0
};

SignalEvent count_event = {
  .m_type_id   = EVENT_TYPE_SIGNAL,
  .m_signal_id = EV_SWITCH,
  .m_data      = NULL,
  .m_data_size = 0
};

Event start_event = {
  .m_type_id = EVENT_TYPE_BASE
};

typedef struct STestMachineContext {
  const StateMachine * const m_state_machine;
  int                        m_counter;
} TestMachineContext;

void
effectStart(
    SignalEvent * t_event,
    void *        t_context );

void
entryOff(
    SignalEvent * t_event,
    void *        t_context );

void
entryOn(
    SignalEvent * t_event,
    void *        t_context);

void
doOnOff(
    SignalEvent * t_event,
    void *        t_context);

void
exitOnOff(
    SignalEvent * t_event,
    void *        t_context );

void
actionOffCount(
    SignalEvent * t_event,
    void *        t_context );

bool
actionOffCountGuard(
    SignalEvent * t_event,
    void *        t_context );

SimpleState state_on;
SimpleState state_off;

TestMachineContext tm_context = {
  .m_state_machine = &state_machine,
  .m_counter       = 0
};

Activity activity_off_count = {
  .m_event  = (Event*)&count_event,
  .m_guard  = actionOffCountGuard,
  .m_action = actionOffCount,
};

Transition transition_off_on = {
  .m_event        = (Event*)&switch_event,
  .m_target_state = (State*)&state_on,
  .m_guard        = NULL,
  .m_effect       = NULL
};

Transition transition_on_off = {
  .m_event        = (Event*)&switch_event,
  .m_target_state = (State*)&state_off,
  .m_guard        = NULL,
  .m_effect       = NULL
};

SimpleState state_off = {
  .m_name        = "State Off",
  .m_type_id     = STATE_TYPE_SIMPLE,
  .m_parent      = &state_machine,
  .m_entry       = entryOff,
  .m_do          = doOnOff,
  .m_exit        = exitOnOff,
  .m_activities  = (Activity*[]) {
      &activity_off_count,
      NULL },
  .m_transitions = (Transition*[]){
      &transition_off_on,
      NULL }
};

SimpleState state_on = {
  .m_name        = "State On",
  .m_type_id     = STATE_TYPE_SIMPLE,
  .m_parent      = &state_machine,
  .m_entry       = entryOn,
  .m_do          = doOnOff,
  .m_exit        = exitOnOff,
  .m_activities  = NULL,
  .m_transitions = (Transition*[]) {
    &transition_on_off,
    NULL }
};

StateMachine state_machine = {
  .m_name               = "Test Machine",
  .m_context            = &tm_context,
  .m_current_state      = NULL,
  .m_initial_transition = {
    .m_event          = &start_event,
    .m_target_state   = (State*)&state_off,
    .m_guard          = NULL,
    .m_effect         = effectStart }
};

//_____________________________________________________________________________
int
main(
    int     t_argc,
    char ** t_argv ) {

  (void) t_argc;
  (void) t_argv;

  int         result       = 0;
  SignalEvent event_none   = {
    .m_type_id   = EVENT_TYPE_SIGNAL,
    .m_signal_id = EV_NONE,
    .m_data      = NULL,
    .m_data_size = 0 };
  SignalEvent event_switch = {
    .m_type_id   = EVENT_TYPE_SIGNAL,
    .m_signal_id = EV_SWITCH,
    .m_data      = NULL,
    .m_data_size = 0 };
  SignalEvent event_count  = {
    .m_type_id   = EVENT_TYPE_SIGNAL,
    .m_signal_id = EV_COUNT,
    .m_data      = NULL,
    .m_data_size = 0 };

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
    SignalEvent * t_event,
    void *        t_context ) {

  printf(
      "%-10s (Event: %2d) entry / switch off\n",
      CONTEXT->m_state_machine->m_current_state->m_name,
      t_event->m_signal_id );
} //entryOff

//_____________________________________________________________________________
void
entryOn(
    SignalEvent * t_event,
    void *        t_context ) {

  printf(
      "%-10s (Event: %2d) entry / switch on\n",
      CONTEXT->m_state_machine->m_current_state->m_name,
      t_event->m_signal_id );
} //entryOn

//_____________________________________________________________________________
void
doOnOff(
    SignalEvent * t_event,
    void *        t_context ) {

  printf(
      "%-10s (Event: %2d) do    /\n",
      CONTEXT->m_state_machine->m_current_state->m_name,
      t_event->m_signal_id );
} //doOnOff

//_____________________________________________________________________________
void
exitOnOff(
    SignalEvent * t_event,
    void *        t_context ) {

  printf(
      "%-10s (Event: %2d) exit  / \n",
      CONTEXT->m_state_machine->m_current_state->m_name,
      t_event->m_signal_id );
} //exitOnOff

//_____________________________________________________________________________
void
actionOffCount(
    SignalEvent * t_event,
    void *        t_context ) {

  (void)t_event;
  TestMachineContext * context =
      CONTEXT->m_state_machine->m_context;

  context->m_counter++;
  printf(
      "%-10s (Event: %2d) action_off_count / tm_context.m_counter++ == %d\n",
      CONTEXT->m_state_machine->m_current_state->m_name,
      t_event->m_signal_id,
      context->m_counter );
} //actionOffCount

//_____________________________________________________________________________
bool
actionOffCountGuard(
    SignalEvent * t_event,
    void *        t_context ) {

  (void)t_event;
  (void)t_context;

  return true;
} //actionOffCountGurad

//_____________________________________________________________________________
void
effectStart(
    SignalEvent * t_event,
    void *        t_context ) {

  printf(
      "Starte die StateMachine \"%s\" mit dem Event Nr. %d.\n",
      CONTEXT->m_state_machine->m_name,
      t_event->m_signal_id );
} //effectStart
