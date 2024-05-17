AP_DECLARE(apr_status_t) ap_rgetline(char **s, apr_size_t n,
                                     apr_size_t *read, request_rec *r,
                                     int fold, apr_bucket_brigade *bb)
{
     
    apr_status_t rv;

    rv = ap_rgetline_core(s, n, read, r, fold, bb);
    if (rv == APR_SUCCESS) {
        ap_xlate_proto_from_ascii(*s, *read);
    }
    return rv;
}
