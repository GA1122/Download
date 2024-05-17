static void event_command_ready(fixed_queue_t *queue, UNUSED_ATTR void *context) {
 if (command_credits > 0) {
 waiting_command_t *wait_entry = fixed_queue_dequeue(queue);
    command_credits--;

    pthread_mutex_lock(&commands_pending_response_lock);
    list_append(commands_pending_response, wait_entry);
    pthread_mutex_unlock(&commands_pending_response_lock);

    low_power_manager->wake_assert();
    packet_fragmenter->fragment_and_dispatch(wait_entry->command);
    low_power_manager->transmit_done();

    non_repeating_timer_restart_if(command_response_timer, !list_is_empty(commands_pending_response));
 }
}
