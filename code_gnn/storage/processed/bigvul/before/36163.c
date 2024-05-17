struct client_session *mk_session_create(int socket, struct sched_list_node *sched)
{
    struct client_session *cs;
    struct sched_connection *sc;

    sc = mk_sched_get_connection(sched, socket);
    if (!sc) {
        MK_TRACE("[FD %i] No sched node, could not create session", socket);
        return NULL;
    }

     
    cs = mk_mem_malloc(sizeof(struct client_session));

    cs->pipelined = MK_FALSE;
    cs->counter_connections = 0;
    cs->socket = socket;
    cs->status = MK_REQUEST_STATUS_INCOMPLETE;
    mk_list_add(&cs->request_incomplete, cs_incomplete);

     
    cs->init_time = sc->arrive_time;

     
    if (config->transport_buffer_size > MK_REQUEST_CHUNK) {
        cs->body = mk_mem_malloc(config->transport_buffer_size);
        cs->body_size = config->transport_buffer_size;
    }
    else {
         
        cs->body = cs->body_fixed;
        cs->body_size = MK_REQUEST_CHUNK;
    }

     
    cs->body_length = 0;

    cs->body_pos_end = -1;
    cs->first_method = MK_HTTP_METHOD_UNKNOWN;

     
    mk_list_init(&cs->request_list);

     

     
     
    struct rb_node **new = &(cs_list->rb_node);
    struct rb_node *parent = NULL;

     
    while (*new) {
        struct client_session *this = container_of(*new, struct client_session, _rb_head);

        parent = *new;
        if (cs->socket < this->socket)
            new = &((*new)->rb_left);
        else if (cs->socket > this->socket)
            new = &((*new)->rb_right);
        else {
            break;
        }
    }
     
    rb_link_node(&cs->_rb_head, parent, new);
    rb_insert_color(&cs->_rb_head, cs_list);

    return cs;
}