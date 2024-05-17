static void local_socket_ready(asocket* s) {
  
    fdevent_add(&s->fde, FDE_READ);
}
