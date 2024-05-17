char *am_get_assertion_consumer_service_by_binding(LassoProvider *provider, const char *binding)
{
    GList *descriptors;
    char *url;
    char *selected_descriptor;
    char *descriptor;
    char **tokens;
    guint n_tokens;
    GList *i;
    char *endptr;
    long descriptor_index, min_index;

    url = NULL;
    selected_descriptor = NULL;
    min_index = LONG_MAX;

     
    descriptors = lasso_provider_get_metadata_keys_for_role(provider,
                                                            LASSO_PROVIDER_ROLE_SP);

    for (i = g_list_first(descriptors), tokens=NULL;
         i;
         i = g_list_next(i), g_strfreev(tokens)) {

        descriptor = i->data;
        descriptor_index = LONG_MAX;

         

        tokens = g_strsplit(descriptor, " ", 0);
        n_tokens = g_strv_length(tokens);

        if (n_tokens < 3) continue;

        if (!g_str_equal(tokens[0], "AssertionConsumerService")) continue;
        if (!g_str_equal(tokens[1], binding)) continue;

        descriptor_index = strtol(tokens[2], &endptr, 10);
        if (tokens[2] == endptr) continue;  

        if (descriptor_index < min_index) {
            selected_descriptor = descriptor;
            min_index = descriptor_index;
        }
    }

    if (selected_descriptor) {
        url = lasso_provider_get_metadata_one_for_role(provider,
                                                       LASSO_PROVIDER_ROLE_SP,
                                                       selected_descriptor);
    }

    lasso_release_list_of_strings(descriptors);

    return url;
}
