int ssl2_pending(const SSL *s)
{
    return SSL_in_init(s) ? 0 : s->s2->ract_data_length;
}
