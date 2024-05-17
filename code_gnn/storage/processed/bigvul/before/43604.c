AP_DECLARE(int) ap_is_initial_req(request_rec *r)
{
    return (r->main == NULL)        
           && (r->prev == NULL);    
}
