reason_to_string (NMDeviceStateReason reason)
{
	if ((gsize) reason < G_N_ELEMENTS (reason_table))
		return reason_table[reason];
	return reason_table[NM_DEVICE_STATE_REASON_UNKNOWN];
}
