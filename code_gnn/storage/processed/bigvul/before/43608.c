AP_DECLARE(int) ap_some_auth_required(request_rec *r)
{
     
    if (ap__authz_ap_some_auth_required) {
        return ap__authz_ap_some_auth_required(r);
    }
    else
        return 0;
}
