int pseudo_get_sec_session_info(
	char const *starter_reconnect_session_info,
	MyString &reconnect_session_id,
	MyString &reconnect_session_info,
	MyString &reconnect_session_key,
	char const *starter_filetrans_session_info,
	MyString &filetrans_session_id,
	MyString &filetrans_session_info,
	MyString &filetrans_session_key)
{
	RemoteResource *remote;
	if (parallelMasterResource == NULL) {
		remote = thisRemoteResource;
	} else {
		remote = parallelMasterResource;
	}

	bool rc = remote->getSecSessionInfo(
		starter_reconnect_session_info,
		reconnect_session_id,
		reconnect_session_info,
		reconnect_session_key,
		starter_filetrans_session_info,
		filetrans_session_id,
		filetrans_session_info,
		filetrans_session_key);

	if( !rc ) {
		return -1;
	}
	return 1;
}
