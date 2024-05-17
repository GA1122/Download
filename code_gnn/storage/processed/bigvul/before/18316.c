test_set_variable (struct EventFixture *ef, const void *data) {
    (void) data;

     
    parse_cmd_line("set useragent = Uzbl browser kthxbye!", NULL);
    ASSERT_EVENT(ef, "VARIABLE_SET useragent str Uzbl browser kthxbye!");
    g_assert_cmpstr("Uzbl browser kthxbye!", ==, uzbl.net.useragent);

     
    parse_cmd_line("set forward_keys = 0", NULL);
    ASSERT_EVENT(ef, "VARIABLE_SET forward_keys int 0");
    g_assert_cmpint(0, ==, uzbl.behave.forward_keys);

     
     
    GString *cmd, *ev;
    cmd = g_string_new("set zoom_level = ");
    g_string_append_printf(cmd, "%f", 0.25);
    parse_cmd_line(g_string_free(cmd, FALSE), NULL);

    ev = g_string_new("EVENT [" INSTANCE_NAME "] VARIABLE_SET zoom_level float ");
    g_string_append_printf(ev, "%f\n", 0.25);
    read_event(ef);
    g_assert_cmpstr(g_string_free(ev, FALSE), ==, ef->event_buffer);

    g_assert_cmpfloat(0.25, ==, uzbl.behave.zoom_level);

     
    int old_major = uzbl.info.webkit_major;
    parse_cmd_line("set WEBKIT_MAJOR = 100", NULL);
    assert_no_event(ef);
    g_assert_cmpint(old_major, ==, uzbl.info.webkit_major);

     
    GString *old_arch = g_string_new(uzbl.info.arch);
    parse_cmd_line("set ARCH_UZBL = A Lisp Machine", NULL);
    assert_no_event(ef);
    g_assert_cmpstr(g_string_free(old_arch, FALSE), ==, uzbl.info.arch);

     
    parse_cmd_line("set nonexistant_variable = Some Value", NULL);
    ASSERT_EVENT(ef, "VARIABLE_SET nonexistant_variable str Some Value");
    uzbl_cmdprop *c = g_hash_table_lookup(uzbl.comm.proto_var, "nonexistant_variable");
    g_assert_cmpstr("Some Value", ==, *c->ptr.s);

     
    parse_cmd_line("set an_expanded_variable = Test @(echo expansion)@", NULL);
    ASSERT_EVENT(ef, "VARIABLE_SET an_expanded_variable str Test expansion");
    c = g_hash_table_lookup(uzbl.comm.proto_var, "an_expanded_variable");
    g_assert_cmpstr("Test expansion", ==, *c->ptr.s);
}
