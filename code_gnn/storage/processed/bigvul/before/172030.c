static void on_l2cap_outgoing_congest(tBTA_JV_L2CAP_CONG *p, uint32_t id)
{
    l2cap_socket *sock;

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (sock) {
        sock->outgoing_congest = p->cong ? 1 : 0;
 if (!sock->outgoing_congest) {
            APPL_TRACE_DEBUG("on_l2cap_outgoing_congest: adding fd to btsock_thread...");
            btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD, sock->id);

 }
 }
    pthread_mutex_unlock(&state_lock);
}
