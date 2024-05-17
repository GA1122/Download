static apr_status_t session_pool_cleanup(void *data)
{
    h2_session *session = data;
     
    ap_log_cerror(APLOG_MARK, APLOG_TRACE1, 0, session->c,
                  "session(%ld): pool_cleanup", session->id);
    
    if (session->state != H2_SESSION_ST_DONE) {
         
        ap_log_cerror(APLOG_MARK, APLOG_WARNING, 0, session->c, APLOGNO(03199)
                      "session(%ld): connection disappeared without proper "
                      "goodbye, clients will be confused, should not happen", 
                      session->id);
    }
     
    session->pool = NULL;
    h2_session_destroy(session);
    return APR_SUCCESS;
}
