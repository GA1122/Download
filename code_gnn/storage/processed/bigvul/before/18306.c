assert_str_beginswith(GString *expected, gchar *actual) {
    gchar *actual_beginning = g_strndup(actual, expected->len);
    g_assert_cmpstr(expected->str, ==, actual_beginning);
    g_free(actual_beginning);

     
    return &actual[expected->len];
}
