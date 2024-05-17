queued_state_to_string (NMDeviceState state)
{
	if ((gsize) state < G_N_ELEMENTS (state_table))
		return state_table[state];
	return state_table[NM_DEVICE_STATE_UNKNOWN];
}
