BOOLEAN BTM_SecRegisterLinkKeyNotificationCallback (tBTM_LINK_KEY_CALLBACK *p_callback)
{
    btm_cb.api.p_link_key_callback = p_callback;
 return(TRUE);
}
