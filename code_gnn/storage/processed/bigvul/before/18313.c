test_run_handler_arg_order (void) {
    run_handler("sync_spawn echo uvw xyz", "abc def");

    assert(uzbl.comm.sync_stdout);

     
    gchar *rest = assert_sync_beginswith_stdarg();

     
     
    g_assert_cmpstr("abc def uvw xyz\n", ==, rest);
}
