h_conn_data(conn c)
{
    int r, to_read;
    job j;
    struct iovec iov[2];

    switch (c->state) {
    case STATE_WANTCOMMAND:
        r = read(c->fd, c->cmd + c->cmd_read, LINE_BUF_SIZE - c->cmd_read);
        if (r == -1) return check_err(c, "read()");
        if (r == 0) return conn_close(c);  

        c->cmd_read += r;  

        c->cmd_len = cmd_len(c);  

         
        if (c->cmd_len) return do_cmd(c);

         

         
        if (c->cmd_read == LINE_BUF_SIZE) {
            c->cmd_read = 0;  
            return reply_msg(c, MSG_BAD_FORMAT);
        }

         
        break;
    case STATE_BITBUCKET:
         
        to_read = min(c->in_job_read, BUCKET_BUF_SIZE);
        r = read(c->fd, bucket, to_read);
        if (r == -1) return check_err(c, "read()");
        if (r == 0) return conn_close(c);  

        c->in_job_read -= r;  

         

        if (c->in_job_read == 0) {
            return reply(c, c->reply, c->reply_len, STATE_SENDWORD);
        }
        break;
    case STATE_WANTDATA:
        j = c->in_job;

        r = read(c->fd, j->body + c->in_job_read, j->body_size -c->in_job_read);
        if (r == -1) return check_err(c, "read()");
        if (r == 0) return conn_close(c);  

        c->in_job_read += r;  

         

        maybe_enqueue_incoming_job(c);
        break;
    case STATE_SENDWORD:
        r= write(c->fd, c->reply + c->reply_sent, c->reply_len - c->reply_sent);
        if (r == -1) return check_err(c, "write()");
        if (r == 0) return conn_close(c);  

        c->reply_sent += r;  

         

        if (c->reply_sent == c->reply_len) return reset_conn(c);

         
        break;
    case STATE_SENDJOB:
        j = c->out_job;

        iov[0].iov_base = (void *)(c->reply + c->reply_sent);
        iov[0].iov_len = c->reply_len - c->reply_sent;  
        iov[1].iov_base = j->body + c->out_job_sent;
        iov[1].iov_len = j->body_size - c->out_job_sent;

        r = writev(c->fd, iov, 2);
        if (r == -1) return check_err(c, "writev()");
        if (r == 0) return conn_close(c);  

         
        c->reply_sent += r;
        if (c->reply_sent >= c->reply_len) {
            c->out_job_sent += c->reply_sent - c->reply_len;
            c->reply_sent = c->reply_len;
        }

         

         
        if (c->out_job_sent == j->body_size) return reset_conn(c);

         
        break;
    case STATE_WAIT:  
         
        if (LINE_BUF_SIZE - c->cmd_read < 1) break;

        r = read(c->fd, c->cmd + c->cmd_read, LINE_BUF_SIZE - c->cmd_read);
        if (r == -1) return check_err(c, "read()");
        if (r == 0) return conn_close(c);  
        c->cmd_read += r;  
    }
}
