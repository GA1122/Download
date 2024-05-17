wifi_error wifi_set_nodfs_flag(wifi_interface_handle handle, u32 nodfs)
{
 SetNodfsCommand command(handle, nodfs);
 return (wifi_error) command.requestResponse();
}
