static enum test_return test_binary_set_impl(const char *key, uint8_t cmd) {
    union {
        protocol_binary_request_no_extras request;
        protocol_binary_response_no_extras response;
        char bytes[1024];
    } send, receive;
    uint64_t value = 0xdeadbeefdeadcafe;
    size_t len = storage_command(send.bytes, sizeof(send.bytes), cmd,
                                 key, strlen(key), &value, sizeof(value),
                                 0, 0);

     
    int ii;
    for (ii = 0; ii < 10; ++ii) {
        safe_send(send.bytes, len, false);
        if (cmd == PROTOCOL_BINARY_CMD_SET) {
            safe_recv_packet(receive.bytes, sizeof(receive.bytes));
            validate_response_header(&receive.response, cmd,
                                     PROTOCOL_BINARY_RESPONSE_SUCCESS);
        }
    }

    if (cmd == PROTOCOL_BINARY_CMD_SETQ) {
        return test_binary_noop();
    }

    send.request.message.header.request.cas = receive.response.message.header.response.cas;
    safe_send(send.bytes, len, false);
    if (cmd == PROTOCOL_BINARY_CMD_SET) {
        safe_recv_packet(receive.bytes, sizeof(receive.bytes));
        validate_response_header(&receive.response, cmd,
                                 PROTOCOL_BINARY_RESPONSE_SUCCESS);
        assert(receive.response.message.header.response.cas != send.request.message.header.request.cas);
    } else {
        return test_binary_noop();
    }

    return TEST_PASS;
}