bt_status_t btsock_l2cap_cleanup()
{
    pthread_mutex_lock(&state_lock);
    pth = -1;
 while (socks)
        btsock_l2cap_free_l(socks);
    pthread_mutex_unlock(&state_lock);

 return BT_STATUS_SUCCESS;
}
