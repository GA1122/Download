test_print (void) {
    GString *result = g_string_new("");

     
    parse_cmd_line("print A simple test", result);
    g_assert_cmpstr("A simple test", ==, result->str);

     
    parse_cmd_line("print A simple @(echo expansion)@ test", result);
    g_assert_cmpstr("A simple expansion test", ==, result->str);

    g_string_free(result, TRUE);
}
