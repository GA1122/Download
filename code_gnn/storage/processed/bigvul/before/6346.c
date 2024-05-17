state_to_string (NMDeviceState state)
{
	return queued_state_to_string (state) + strlen (QUEUED_PREFIX);
}
