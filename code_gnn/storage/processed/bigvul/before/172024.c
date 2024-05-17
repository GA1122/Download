static void on_cl_l2cap_init(tBTA_JV_L2CAP_CL_INIT *p_init, uint32_t id)
{
    l2cap_socket *sock;

    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
 if (sock) {
 if (p_init->status != BTA_JV_SUCCESS) {
            btsock_l2cap_free_l(sock);
 } else {
            sock->handle = p_init->handle;
 }
 }
    pthread_mutex_unlock(&state_lock);
}
