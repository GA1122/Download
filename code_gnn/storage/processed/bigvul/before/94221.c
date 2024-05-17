static void *binary_hickup_recv_verification_thread(void *arg) {
    protocol_binary_response_no_extras *response = malloc(65*1024);
    if (response != NULL) {
        while (safe_recv_packet(response, 65*1024)) {
             
            validate_response_header(response,
                                     response->message.header.response.opcode,
                                     response->message.header.response.status);
        }
        free(response);
    }
    hickup_thread_running = false;
    allow_closed_read = false;
    return NULL;
}