logger_timer_cb (const void *pointer, void *data, int remaining_calls)
{
     
    (void) pointer;
    (void) data;
    (void) remaining_calls;

    logger_flush ();

    return WEECHAT_RC_OK;
}
