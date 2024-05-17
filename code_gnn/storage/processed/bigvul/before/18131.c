do_list_tubes(conn c, ms l)
{
    char *buf;
    tube t;
    size_t i, resp_z;

     
    resp_z = 6;  
    for (i = 0; i < l->used; i++) {
        t = l->items[i];
        resp_z += 3 + strlen(t->name);  
    }

    c->out_job = allocate_job(resp_z);  
    if (!c->out_job) return reply_serr(c, MSG_OUT_OF_MEMORY);

     
    c->out_job->state = JOB_STATE_COPY;

     
    buf = c->out_job->body;
    buf += snprintf(buf, 5, "---\n");
    for (i = 0; i < l->used; i++) {
        t = l->items[i];
        buf += snprintf(buf, 4 + strlen(t->name), "- %s\n", t->name);
    }
    buf[0] = '\r';
    buf[1] = '\n';

    c->out_job_sent = 0;
    return reply_line(c, STATE_SENDJOB, "OK %d\r\n", resp_z - 2);
}
