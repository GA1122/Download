AP_DECLARE(void) ap_destroy_sub_req(request_rec *r)
{
     
    apr_pool_destroy(r->pool);
}
