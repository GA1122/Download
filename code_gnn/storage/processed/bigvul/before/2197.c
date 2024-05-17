static void red_peer_handle_incoming(RedsStream *stream, IncomingHandler *handler)
{
    int bytes_read;
    uint8_t *parsed;
    size_t parsed_size;
    message_destructor_t parsed_free;
    uint16_t msg_type;
    uint32_t msg_size;

     
    if (!stream) {
        return;
    }

    for (;;) {
        int ret_handle;
        if (handler->header_pos < handler->header.header_size) {
            bytes_read = red_peer_receive(stream,
                                          handler->header.data + handler->header_pos,
                                          handler->header.header_size - handler->header_pos);
            if (bytes_read == -1) {
                handler->cb->on_error(handler->opaque);
                return;
            }
            handler->header_pos += bytes_read;

            if (handler->header_pos != handler->header.header_size) {
                return;
            }
        }

        msg_size = handler->header.get_msg_size(&handler->header);
        msg_type = handler->header.get_msg_type(&handler->header);
        if (handler->msg_pos < msg_size) {
            if (!handler->msg) {
                handler->msg = handler->cb->alloc_msg_buf(handler->opaque, msg_type, msg_size);
                if (handler->msg == NULL) {
                    spice_printerr("ERROR: channel refused to allocate buffer.");
                    handler->cb->on_error(handler->opaque);
                    return;
                }
            }

            bytes_read = red_peer_receive(stream,
                                          handler->msg + handler->msg_pos,
                                          msg_size - handler->msg_pos);
            if (bytes_read == -1) {
                handler->cb->release_msg_buf(handler->opaque, msg_type, msg_size, handler->msg);
                handler->cb->on_error(handler->opaque);
                return;
            }
            handler->msg_pos += bytes_read;
            if (handler->msg_pos != msg_size) {
                return;
            }
        }

        if (handler->cb->parser) {
            parsed = handler->cb->parser(handler->msg,
                handler->msg + msg_size, msg_type,
                SPICE_VERSION_MINOR, &parsed_size, &parsed_free);
            if (parsed == NULL) {
                spice_printerr("failed to parse message type %d", msg_type);
                handler->cb->release_msg_buf(handler->opaque, msg_type, msg_size, handler->msg);
                handler->cb->on_error(handler->opaque);
                return;
            }
            ret_handle = handler->cb->handle_parsed(handler->opaque, parsed_size,
                                                    msg_type, parsed);
            parsed_free(parsed);
        } else {
            ret_handle = handler->cb->handle_message(handler->opaque, msg_type, msg_size,
                                                     handler->msg);
        }
        handler->msg_pos = 0;
        handler->cb->release_msg_buf(handler->opaque, msg_type, msg_size, handler->msg);
        handler->msg = NULL;
        handler->header_pos = 0;

        if (!ret_handle) {
            handler->cb->on_error(handler->opaque);
            return;
        }
    }
}
