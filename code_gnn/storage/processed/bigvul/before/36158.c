void mk_request_ka_next(struct client_session *cs)
{
    cs->first_method = -1;
    cs->body_pos_end = -1;
    cs->body_length = 0;
    cs->counter_connections++;

     
    cs->init_time = log_current_utime;
    cs->status = MK_REQUEST_STATUS_INCOMPLETE;
    mk_list_add(&cs->request_incomplete, cs_incomplete);
}