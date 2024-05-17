GahpClient::GahpClient(const char *id, const char *path, const ArgList *args)
{
	server = GahpServer::FindOrCreateGahpServer(id,path,args);
	m_timeout = 0;
	m_mode = normal;
	pending_command = NULL;
	pending_args = NULL;
	pending_reqid = 0;
	pending_result = NULL;
	pending_timeout = 0;
	pending_timeout_tid = -1;
	pending_submitted_to_gahp = false;
	pending_proxy = NULL;
	user_timerid = -1;
	normal_proxy = NULL;
	deleg_proxy = NULL;
	error_string = "";

	server->AddGahpClient();
}
