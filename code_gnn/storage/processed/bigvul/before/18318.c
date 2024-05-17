test_toggle_status (void) {
    g_assert(!uzbl.behave.show_status);

     
    parse_cmd_line("toggle_status", NULL);
    g_assert(uzbl.behave.show_status);

     
    parse_cmd_line("toggle_status", NULL);
    g_assert(!uzbl.behave.show_status);
}
