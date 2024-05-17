GahpClient::is_pending(const char *command, const char *  ) 
{
	if ( command && pending_command && strcmp(command,pending_command)==0 )
	{
		return true;
	} 

	return false;
}
