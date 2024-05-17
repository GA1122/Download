static struct pending_auth_data *get_pending_auth_data(
		struct smbd_server_connection *sconn,
		uint16_t smbpid)
{
	struct pending_auth_data *pad;
 
	for (pad = sconn->smb1.pd_list; pad; pad = pad->next) {
		if (pad->smbpid == smbpid) {
			break;
		}
	}
	return pad;
}
