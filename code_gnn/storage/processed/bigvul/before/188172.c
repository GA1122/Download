 void close_all_sockets(atransport* t) {
     asocket* s;
 
 
       
    adb_mutex_lock(&socket_list_lock);
//     std::lock_guard<std::recursive_mutex> lock(local_socket_list_lock);
  restart:
      for (s = local_socket_list.next; s != &local_socket_list; s = s->next) {
          if (s->transport == t || (s->peer && s->peer->transport == t)) {
            local_socket_close_locked(s);
//             local_socket_close(s);
              goto restart;
          }
      }
    adb_mutex_unlock(&socket_list_lock);
  }