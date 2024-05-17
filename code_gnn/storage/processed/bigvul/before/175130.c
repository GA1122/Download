static int wifi_add_membership(wifi_handle handle, const char *group)
{
    hal_info *info = getHalInfo(handle);

 int id = wifi_get_multicast_id(handle, "nl80211", group);
 if (id < 0) {
        ALOGE("Could not find group %s", group);
 return id;
 }

 int ret = nl_socket_add_membership(info->event_sock, id);
 if (ret < 0) {
        ALOGE("Could not add membership to group %s", group);
 }

 return ret;
}
