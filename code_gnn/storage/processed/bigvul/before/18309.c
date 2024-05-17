event_fixture_teardown(struct EventFixture *ef, const void *data)
{
    (void) data;

     
    assert_no_event(ef);

     
    GIOChannel *iochan = g_ptr_array_index(uzbl.comm.client_chan, 0);
    remove_socket_from_array(iochan);

     
    close(ef->uzbl_sock);
    close(ef->test_sock);
}
