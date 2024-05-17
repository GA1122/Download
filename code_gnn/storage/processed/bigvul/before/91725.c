void am_set_cache_control_headers(request_rec *r)
{
     
    apr_table_setn(r->err_headers_out,
                   "Cache-Control", "private, max-age=0, must-revalidate");
}
