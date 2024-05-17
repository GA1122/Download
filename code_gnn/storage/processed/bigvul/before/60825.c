logger_buffer_closing_signal_cb (const void *pointer, void *data,
                                 const char *signal,
                                 const char *type_data, void *signal_data)
{
     
    (void) pointer;
    (void) data;
    (void) signal;
    (void) type_data;

    logger_stop (logger_buffer_search_buffer (signal_data), 1);

    return WEECHAT_RC_OK;
}
