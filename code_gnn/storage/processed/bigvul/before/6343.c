nm_device_ipv6_sysctl_set (NMDevice *self, const char *property, const char *value)
{
	return nm_platform_sysctl_set (nm_utils_ip6_property_path (nm_device_get_ip_iface (self), property), value);
 	return nm_platform_sysctl_set (nm_utils_ip6_property_path (nm_device_get_ip_iface (self), property), value);
 }
