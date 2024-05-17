wifi_error wifi_get_concurrency_matrix(wifi_interface_handle handle, int set_size_max,
       feature_set set[], int *set_size)
{
 GetFeatureSetCommand command(handle, FEATURE_SET_MATRIX, NULL, set, set_size, set_size_max);
 return (wifi_error) command.requestResponse();
}
