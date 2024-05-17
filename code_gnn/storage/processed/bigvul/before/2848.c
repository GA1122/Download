update_one_secret (NMSetting *setting, const char *key, GValue *value, GError **error)
{
	gboolean success = FALSE;

	g_return_val_if_fail (key != NULL, FALSE);
	g_return_val_if_fail (value != NULL, FALSE);

	if (G_VALUE_HOLDS_STRING (value)) {
		 
		success = update_secret_string (setting, key, g_value_get_string (value), error);
	} else if (G_VALUE_HOLDS (value, DBUS_TYPE_G_MAP_OF_STRING)) {
		if (strcmp (key, NM_SETTING_VPN_SECRETS) != 0) {
			g_set_error (error, NM_SETTING_ERROR, NM_SETTING_ERROR_PROPERTY_NOT_SECRET,
			             "Property %s not a secret property", key);
		} else
			success = update_secret_hash (setting, g_value_get_boxed (value), error);
	} else
		g_set_error_literal (error, NM_SETTING_ERROR, NM_SETTING_ERROR_PROPERTY_TYPE_MISMATCH, key);

	return success;
}
