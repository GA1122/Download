void boa_perror(request * req, const char *message)
{
    log_error_doc(req);
    perror(message);             
    send_r_error(req);
}
