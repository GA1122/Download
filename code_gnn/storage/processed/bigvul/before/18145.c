h_conn(const int fd, const short which, conn c)
{
    if (fd != c->fd) {
        twarnx("Argh! event fd doesn't match conn fd.");
        close(fd);
        return conn_close(c);
    }

    switch (which) {
    case EV_TIMEOUT:
        h_conn_timeout(c);
        event_add(&c->evq, NULL);  
        break;
    case EV_READ:
         
    case EV_WRITE:
         
    default:
        h_conn_data(c);
    }

    while (cmd_data_ready(c) && (c->cmd_len = cmd_len(c))) do_cmd(c);
}