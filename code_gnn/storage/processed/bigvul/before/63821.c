httpd_start_request( httpd_conn* hc, struct timeval* nowP )
    {
    int r;

     
    r = really_start_request( hc, nowP );

     
    return r;
    }
