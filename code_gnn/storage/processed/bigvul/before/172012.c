static l2cap_socket *btsock_l2cap_alloc(const char *name, const bt_bdaddr_t *addr,
 char is_server, int flags)
{
    l2cap_socket *ret;

    pthread_mutex_lock(&state_lock);
    ret = btsock_l2cap_alloc_l(name, addr, is_server, flags);
    pthread_mutex_unlock(&state_lock);

 return ret;
}
