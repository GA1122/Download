wifi_error wifi_get_supported_feature_set(wifi_interface_handle handle, feature_set *set)
{
 GetFeatureSetCommand command(handle, FEATURE_SET, set, NULL, NULL, 1);
 return (wifi_error) command.requestResponse();
}
