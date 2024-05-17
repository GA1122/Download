_skip(conn c, int n, const char *line, int len)
{
     
    c->in_job = 0;
    c->in_job_read = n;
    fill_extra_data(c);

    if (c->in_job_read == 0) return reply(c, line, len, STATE_SENDWORD);

    c->reply = line;
    c->reply_len = len;
    c->reply_sent = 0;
    c->state = STATE_BITBUCKET;
    return;
}
