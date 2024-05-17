void BTM_SecSetConnectFilterCallback (tBTM_FILTER_CB *p_callback)
{
    btm_cb.p_conn_filter_cb = p_callback;
}
