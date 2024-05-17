unix_client_disconnect(hsm_com_client_hdl_t *hdl)
{
	if(unix_sck_send_disconnect(hdl, 2) != HSM_COM_OK)
	{
		return(-1);
	}

	close(hdl->client_fd);
	hdl->client_state = HSM_COM_C_STATE_IN;


	return HSM_COM_OK;

}