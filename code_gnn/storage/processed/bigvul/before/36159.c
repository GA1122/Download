static int mk_request_parse(struct client_session *cs)
{
    int i, end;
    int blocks = 0;
    struct session_request *sr_node;
    struct mk_list *sr_list, *sr_head;

    for (i = 0; i <= cs->body_pos_end; i++) {
         
        end = mk_string_search(cs->body + i, mk_endblock.data, MK_STR_SENSITIVE) + i;
        if (end <  0) {
            return -1;
        }

         
        if (blocks == 0) {
            sr_node = &cs->sr_fixed;
            memset(sr_node, '\0', sizeof(struct session_request));
        }
        else {
            sr_node = mk_mem_malloc_z(sizeof(struct session_request));
        }
        mk_request_init(sr_node);

         
        sr_node->body.data = cs->body + i;
        sr_node->body.len = end - i;

         
        if (i == 0) {
            sr_node->method = cs->first_method;
        }
        else {
            sr_node->method = mk_http_method_get(sr_node->body.data);
        }

         
        if (sr_node->method == MK_HTTP_METHOD_POST) {
            int offset;
            offset = end + mk_endblock.len;
            sr_node->data = mk_method_get_data(cs->body + offset,
                                               cs->body_length - offset);
        }

         
        i = (end + mk_endblock.len) - 1;

         
        mk_list_add(&sr_node->_head, &cs->request_list);

         
        blocks++;
    }

     

     
    if (blocks > 1) {
        sr_list = &cs->request_list;
        mk_list_foreach(sr_head, sr_list) {
            sr_node = mk_list_entry(sr_head, struct session_request, _head);
             
            if (sr_node->method != MK_HTTP_METHOD_GET &&
                sr_node->method != MK_HTTP_METHOD_HEAD) {
                return -1;
            }
        }
        cs->pipelined = MK_TRUE;
    }

#ifdef TRACE
    int b = 0;
    if (cs->pipelined == MK_TRUE) {
        MK_TRACE("[FD %i] Pipeline Requests: %i blocks", cs->socket, blocks);
        sr_list = &cs->request_list;
        mk_list_foreach(sr_head, sr_list) {
            sr_node = mk_list_entry(sr_head, struct session_request, _head);
            MK_TRACE("[FD %i] Pipeline Block #%i: %p", cs->socket, b, sr_node);
            b++;
        }
    }
#endif

    return 0;
}
