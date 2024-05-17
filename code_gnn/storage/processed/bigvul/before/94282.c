static enum test_return test_issue_92(void) {
    char buffer[1024];

    close(sock);
    sock = connect_server("127.0.0.1", port, false);

    send_ascii_command("stats cachedump 1 0 0\r\n");
    read_ascii_response(buffer, sizeof(buffer));
    assert(strncmp(buffer, "END", strlen("END")) == 0);

    send_ascii_command("stats cachedump 200 0 0\r\n");
    read_ascii_response(buffer, sizeof(buffer));
    assert(strncmp(buffer, "CLIENT_ERROR", strlen("CLIENT_ERROR")) == 0);

    close(sock);
    sock = connect_server("127.0.0.1", port, false);
     return TEST_PASS;
 }