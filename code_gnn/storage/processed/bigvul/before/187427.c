 void close_all_sockets(atransport* t) {
     asocket* s;
 
   
     std::lock_guard<std::recursive_mutex> lock(local_socket_list_lock);
 
  restart:
      for (s = local_socket_list.next; s != &local_socket_list; s = s->next) {
          if (s->transport == t || (s->peer && s->peer->transport == t)) {
            local_socket_close(s);
//             s->close(s);
              goto restart;
          }
      }
 }