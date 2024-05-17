static void nbd_send_opt_abort(QIOChannel *ioc)
{
     
    nbd_send_option_request(ioc, NBD_OPT_ABORT, 0, NULL, NULL);
}
