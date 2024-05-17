void on_l2cap_psm_assigned(int id, int psm) {
    l2cap_socket *sock;
  
    pthread_mutex_lock(&state_lock);
    sock = btsock_l2cap_find_by_id_l(id);
    sock->channel = psm;

 if(btSock_start_l2cap_server_l(sock) != BT_STATUS_SUCCESS) {
        btsock_l2cap_free_l(sock);
 }

    pthread_mutex_unlock(&state_lock);

}
