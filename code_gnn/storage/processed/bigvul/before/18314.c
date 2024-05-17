test_run_handler_expand (void) {
    uzbl.net.useragent = "Test uzbl uzr agent";
    run_handler("sync_spawn echo @useragent", "result:");

    assert(uzbl.comm.sync_stdout);

     
    gchar *rest = assert_sync_beginswith_stdarg();

     
     
    g_assert_cmpstr("result: Test uzbl uzr agent\n", ==, rest);
}
