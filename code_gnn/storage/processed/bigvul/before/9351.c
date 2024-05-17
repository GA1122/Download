static info_cb get_callback(SSL *s)
{
    if (s->info_callback != NULL)
        return s->info_callback;
    else if (s->ctx->info_callback != NULL)
        return s->ctx->info_callback;

    return NULL;
}
