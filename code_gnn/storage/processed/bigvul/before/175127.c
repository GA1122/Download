static int internal_pollin_handler(wifi_handle handle)
{
    hal_info *info = getHalInfo(handle);
 struct nl_cb *cb = nl_socket_get_cb(info->event_sock);
 int res = nl_recvmsgs(info->event_sock, cb);
    nl_cb_put(cb);
 return res;
}
