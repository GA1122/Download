 static enum test_return start_memcached_server(void) {
     server_pid = start_server(&port, false, 600);
     sock = connect_server("127.0.0.1", port, false);
    return TEST_PASS;
}