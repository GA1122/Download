maybe_enqueue_incoming_job(conn c)
{
    job j = c->in_job;

     
    if (c->in_job_read == j->body_size) return enqueue_incoming_job(c);

     
    c->state = STATE_WANTDATA;
}
