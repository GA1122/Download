static int wifi_get_multicast_id(wifi_handle handle, const char *name, const char *group)
{
 GetMulticastIdCommand cmd(handle, name, group);
 int res = cmd.requestResponse();
 if (res < 0)
 return res;
 else
 return cmd.getId();
}
