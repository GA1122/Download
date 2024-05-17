AP_DECLARE(int) ap_should_client_block(request_rec *r)
{
     

    if (r->read_length || (!r->read_chunked && (r->remaining <= 0))) {
        return 0;
    }

    return 1;
}
