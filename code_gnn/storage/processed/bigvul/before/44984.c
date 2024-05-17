AP_DECLARE(void) ap_finalize_sub_req_protocol(request_rec *sub)
{
     
    if (!sub->eos_sent) {
        end_output_stream(sub);
    }
}
