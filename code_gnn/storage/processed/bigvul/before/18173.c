reply_job(conn c, job j, const char *word)
{
     
    c->out_job = j;
    c->out_job_sent = 0;

    return reply_line(c, STATE_SENDJOB, "%s %llu %u\r\n",
                      word, j->id, j->body_size - 2);
}
