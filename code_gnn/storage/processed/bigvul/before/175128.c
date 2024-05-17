static int internal_valid_message_handler(nl_msg *msg, void *arg)
{

    wifi_handle handle = (wifi_handle)arg;
    hal_info *info = getHalInfo(handle);

 WifiEvent event(msg);
 int res = event.parse();
 if (res < 0) {
        ALOGE("Failed to parse event: %d", res);
 return NL_SKIP;
 }

 int cmd = event.get_cmd();
 uint32_t vendor_id = 0;
 int subcmd = 0;

 if (cmd == NL80211_CMD_VENDOR) {
        vendor_id = event.get_u32(NL80211_ATTR_VENDOR_ID);
        subcmd = event.get_u32(NL80211_ATTR_VENDOR_SUBCMD);
        ALOGV("event received %s, vendor_id = 0x%0x, subcmd = 0x%0x",
                event.get_cmdString(), vendor_id, subcmd);
 } else {
 }


 bool dispatched = false;

    pthread_mutex_lock(&info->cb_lock);

 for (int i = 0; i < info->num_event_cb; i++) {
 if (cmd == info->event_cb[i].nl_cmd) {
 if (cmd == NL80211_CMD_VENDOR
 && ((vendor_id != info->event_cb[i].vendor_id)
 || (subcmd != info->event_cb[i].vendor_subcmd)))
 {
  
 continue;
 }

            cb_info *cbi = &(info->event_cb[i]);
 nl_recvmsg_msg_cb_t cb_func = cbi->cb_func;
 void *cb_arg = cbi->cb_arg;
 WifiCommand *cmd = (WifiCommand *)cbi->cb_arg;
 if (cmd != NULL) {
                cmd->addRef();
 }
            pthread_mutex_unlock(&info->cb_lock);
 if (cb_func)
 (*cb_func)(msg, cb_arg);
 if (cmd != NULL) {
                cmd->releaseRef();
 }

 return NL_OK;
 }
 }

    pthread_mutex_unlock(&info->cb_lock);
 return NL_OK;
}
