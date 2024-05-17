assert_no_event (struct EventFixture *ef) {
    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(ef->test_sock, &rfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

     
    int res = select(ef->test_sock + 1, &rfds, NULL, NULL, &timeout);

    if(res == 0) {
         

         
        return;
    } else if(res == -1) {
         
        perror("select():");
        assert(0);
    } else {
         
        read_event(ef);
        g_assert_cmpstr("", ==, ef->event_buffer);
    }
}