static apr_status_t h2_session_read(h2_session *session, int block)
{
    apr_status_t status, rstatus = APR_EAGAIN;
    conn_rec *c = session->c;
    apr_off_t read_start = session->io.bytes_read;
    
    while (1) {
         
        status = ap_get_brigade(c->input_filters,
                                session->bbtmp, AP_MODE_READBYTES,
                                block? APR_BLOCK_READ : APR_NONBLOCK_READ,
                                APR_BUCKET_BUFF_SIZE);
         
        apr_brigade_cleanup(session->bbtmp); 

        switch (status) {
            case APR_SUCCESS:
                 
                rstatus = APR_SUCCESS;
                if (block) {
                     
                    block = 0;
                }
                break;
            case APR_EAGAIN:
                return rstatus;
            case APR_TIMEUP:
                return status;
            default:
                if (session->io.bytes_read == read_start) {
                     
                    if (APR_STATUS_IS_ETIMEDOUT(status)
                        || APR_STATUS_IS_ECONNABORTED(status)
                        || APR_STATUS_IS_ECONNRESET(status)
                        || APR_STATUS_IS_EOF(status)
                        || APR_STATUS_IS_EBADF(status)) {
                         
                        ap_log_cerror( APLOG_MARK, APLOG_TRACE1, status, c,
                                      "h2_session(%ld): input gone", session->id);
                    }
                    else {
                         
                        ap_log_cerror( APLOG_MARK, APLOG_DEBUG, status, c,
                                      APLOGNO(02950) 
                                      "h2_session(%ld): error reading, terminating",
                                      session->id);
                    }
                    return status;
                }
                 
                return rstatus;
        }
        if ((session->io.bytes_read - read_start) > (64*1024)) {
             
            ap_log_cerror(APLOG_MARK, APLOG_TRACE2, status, c,
                          "h2_session(%ld): read 64k, returning", session->id);
            break;
        }
    }
    return rstatus;
}
