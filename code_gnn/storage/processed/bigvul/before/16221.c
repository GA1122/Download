GahpServer::pipe_ready(int    )
{
	skip_next_r = true;
	poll_real_soon();
	return TRUE;
}
