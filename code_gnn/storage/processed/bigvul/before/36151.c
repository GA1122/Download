int mk_handler_write(int socket, struct client_session *cs)
{
    int final_status = 0;
    struct session_request *sr_node;
    struct mk_list *sr_list;

    if (mk_list_is_empty(&cs->request_list) == 0) {
        if (mk_request_parse(cs) != 0) {
            return -1;
        }
    }

    sr_list = &cs->request_list;

    sr_node = mk_list_entry_first(sr_list, struct session_request, _head);

    if (sr_node->bytes_to_send > 0) {
         
        final_status = mk_http_send_file(cs, sr_node);
    }
    else if (sr_node->bytes_to_send < 0) {
        final_status = mk_request_process(cs, sr_node);
    }

     
    if (final_status > 0) {
        return final_status;
    }
    else {
         
        mk_plugin_stage_run(MK_PLUGIN_STAGE_40, socket,
                            NULL, cs, sr_node);
        switch (final_status) {
        case EXIT_NORMAL:
        case EXIT_ERROR:
            if (sr_node->close_now == MK_TRUE) {
                return -1;
            }
            break;
        case EXIT_ABORT:
            return -1;
        }
    }

     
    return 0;
}
