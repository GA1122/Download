nm_ip4_config_capture_resolv_conf (GArray *nameservers,
                                   const char *rc_contents)
{
	GPtrArray *read_ns;
	guint i, j;
	gboolean changed = FALSE;

	g_return_val_if_fail (nameservers != NULL, FALSE);

	read_ns = nm_utils_read_resolv_conf_nameservers (rc_contents);
	if (!read_ns)
		return FALSE;

	for (i = 0; i < read_ns->len; i++) {
		const char *s = g_ptr_array_index (read_ns, i);
		guint32 ns = 0;

		if (!inet_pton (AF_INET, s, (void *) &ns) || !ns)
			continue;

		 
		for (j = 0; j < nameservers->len; j++) {
			if (g_array_index (nameservers, guint32, j) == ns)
				break;
		}

		if (j == nameservers->len) {
			g_array_append_val (nameservers, ns);
			changed = TRUE;
		}
	}

	g_ptr_array_unref (read_ns);
	return changed;
}
