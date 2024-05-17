static void on_l2cap_write_done(void* req_id, uint32_t id)
{
    l2cap_socket *sock;

 if (req_id != NULL) {
        osi_free(req_id);  
 }

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (sock && !sock->outgoing_congest) {
        APPL_TRACE_DEBUG("on_l2cap_write_done: adding fd to btsock_thread...");
        btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD, sock->id);
 }
    pthread_mutex_unlock(&state_lock);
}
