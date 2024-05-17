unix_sck_send_conn(hsm_com_client_hdl_t *hdl, int timeout)
{
	hsm_com_con_data_t	msg;

	memset(&msg,0,sizeof(msg));

	msg.header.cmd = HSM_COM_CMD_CONN;
	msg.header.ver = HSM_COM_VER;
	msg.header.trans_id = hdl->trans_id++;
	msg.header.payload_len = sizeof(msg.key);

	msg.key = HSM_COM_KEY;


	if(unix_sck_send_msg(hdl, (char*)&msg, sizeof(msg), (char*)&msg, 
						 sizeof(msg), timeout) != sizeof(msg))
	{
		close(hdl->client_fd);
		hdl->client_state = HSM_COM_C_STATE_IN;

		return HSM_COM_BAD;
	}

	if(msg.header.resp_code == HSM_COM_RESP_OK){
		return HSM_COM_OK;
	}

	return HSM_COM_BAD;
}