nm_setting_vpn_error_get_type (void)
{
	static GType etype = 0;

	if (etype == 0) {
		static const GEnumValue values[] = {
			 
			ENUM_ENTRY (NM_SETTING_VPN_ERROR_UNKNOWN, "UnknownError"),
			 
			ENUM_ENTRY (NM_SETTING_VPN_ERROR_INVALID_PROPERTY, "InvalidProperty"),
			 
			ENUM_ENTRY (NM_SETTING_VPN_ERROR_MISSING_PROPERTY, "MissingProperty"),
			{ 0, 0, 0 }
		};
		etype = g_enum_register_static ("NMSettingVpnError", values);
	}
	return etype;
}
