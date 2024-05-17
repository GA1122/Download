static enum test_return test_binary_getq_impl(const char *key, uint8_t cmd) {
    const char *missing = "test_binary_getq_missing";
    union {
        protocol_binary_request_no_extras request;
        protocol_binary_response_no_extras response;
        char bytes[1024];
    } send, temp, receive;
    size_t len = storage_command(send.bytes, sizeof(send.bytes),
                                 PROTOCOL_BINARY_CMD_ADD,
                                 key, strlen(key), NULL, 0,
                                 0, 0);
    size_t len2 = raw_command(temp.bytes, sizeof(temp.bytes), cmd,
                             missing, strlen(missing), NULL, 0);
     
    temp.request.message.header.request.opaque = 0xfeedface;
    memcpy(send.bytes + len, temp.bytes, len2);
    len += len2;

    len2 = raw_command(temp.bytes, sizeof(temp.bytes), cmd,
                       key, strlen(key), NULL, 0);
    memcpy(send.bytes + len, temp.bytes, len2);
    len += len2;

    safe_send(send.bytes, len, false);
    safe_recv_packet(receive.bytes, sizeof(receive.bytes));
    validate_response_header(&receive.response, PROTOCOL_BINARY_CMD_ADD,
                             PROTOCOL_BINARY_RESPONSE_SUCCESS);
     
    safe_recv_packet(receive.bytes, sizeof(receive.bytes));
    validate_response_header(&receive.response, cmd,
                             PROTOCOL_BINARY_RESPONSE_SUCCESS);

    return TEST_PASS;
}