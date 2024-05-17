bt_status_t btsock_l2cap_init(int handle)
{
    APPL_TRACE_DEBUG("btsock_l2cap_init...");
    pthread_mutex_lock(&state_lock);
    pth = handle;
    socks = NULL;
    pthread_mutex_unlock(&state_lock);

 return BT_STATUS_SUCCESS;
}
