AP_DECLARE(apr_time_t) ap_rationalize_mtime(request_rec *r, apr_time_t mtime)
{
    apr_time_t now;

     
    now = (mtime < r->request_time) ? r->request_time : apr_time_now();
    return (mtime > now) ? now : mtime;
}
