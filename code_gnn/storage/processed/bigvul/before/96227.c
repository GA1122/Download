unix_sck_send_msg(hsm_com_client_hdl_t *hdl, char *snd_buf, int snd_len, char *rcv_buf, int rcv_len, int timeout)
{
	int					nread = 0;
	int					n;
	fd_set				rset;
	struct timeval		tm;
	int					offset = 0;

	if (write(hdl->client_fd,snd_buf,snd_len)<0) {
		printf("return failed.\n");
		return 0;
	} 


	tm.tv_sec = timeout;
	tm.tv_usec = 0;

	FD_ZERO(&rset);


	FD_SET(hdl->client_fd,&rset);

	while(1)
	{

		if ( (n = select(hdl->client_fd + 1,&rset,NULL,NULL,&tm)) < 0){
			return 0;
		}

		if (FD_ISSET(hdl->client_fd, &rset)) 
		{
		
			if ( (nread = unix_sck_read_data(hdl->client_fd,
											 &hdl->scr,
											 hdl->recv_buf, 
											 hdl->buf_len,
											 &offset)) > 0)
			{
				if(nread <= rcv_len){
					memcpy(rcv_buf,hdl->recv_buf,nread);
					return nread;
				}
				printf("response too big\n");
				return 0;
			}
			else if(nread < 0)
			{
				printf("Skipping since we need more data\n");
				continue;
			}
			else
			{
				printf("Response is 0\n");
				return 0;
			}
		}
		
		
	}


	return nread;
}