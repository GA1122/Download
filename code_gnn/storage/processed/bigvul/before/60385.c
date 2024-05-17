status_to_message(u_int32_t status)
{
	const char *status_messages[] = {
		"Success",			 
		"End of file",			 
		"No such file",			 
		"Permission denied",		 
		"Failure",			 
		"Bad message",			 
		"No connection",		 
		"Connection lost",		 
		"Operation unsupported",	 
		"Unknown error"			 
	};
	return (status_messages[MINIMUM(status,SSH2_FX_MAX)]);
}
