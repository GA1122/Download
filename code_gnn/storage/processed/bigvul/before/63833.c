post_post_garbage_hack( httpd_conn* hc )
    {
    char buf[2];

     
    if ( sub_process )
	httpd_set_ndelay( hc->conn_fd );
     
    (void) read( hc->conn_fd, buf, sizeof(buf) );
    }
