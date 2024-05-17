static waiting_command_t *get_waiting_command(command_opcode_t opcode) {
  pthread_mutex_lock(&commands_pending_response_lock);

 for (const list_node_t *node = list_begin(commands_pending_response);
      node != list_end(commands_pending_response);
      node = list_next(node)) {
 waiting_command_t *wait_entry = list_node(node);

 if (!wait_entry || wait_entry->opcode != opcode)
 continue;

    list_remove(commands_pending_response, wait_entry);

    pthread_mutex_unlock(&commands_pending_response_lock);
 return wait_entry;
 }

  pthread_mutex_unlock(&commands_pending_response_lock);
 return NULL;
}
