wifi_error wifi_set_country_code(wifi_interface_handle handle, const char *country_code)
{
 SetCountryCodeCommand command(handle, country_code);
 return (wifi_error) command.requestResponse();
}
