httpd_write_response( httpd_conn* hc )
    {
     
    if ( sub_process )
	httpd_clear_ndelay( hc->conn_fd );
     
    if ( hc->responselen > 0 )
	{
	(void) httpd_write_fully( hc->conn_fd, hc->response, hc->responselen );
	hc->responselen = 0;
	}
    }
