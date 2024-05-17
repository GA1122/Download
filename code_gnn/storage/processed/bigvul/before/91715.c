bool am_header_has_media_type(request_rec *r, const char *header, const char *media_type)
{
    bool result = false;
    char **comma_tokens = NULL;
    char **media_ranges = NULL;
    char *media_range = NULL;

    if (header == NULL) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                     "invalid Accept header, NULL");
        goto cleanup;
    }

     
    comma_tokens = g_strsplit(header, ",", 0);
    for (media_ranges = comma_tokens, media_range = *media_ranges;
         media_range;
         media_range = *(++media_ranges)) {
        char **semicolon_tokens = NULL;
        char *name = NULL;

         
        media_range = g_strstrip(media_range);
        semicolon_tokens = g_strsplit(media_range, ";", 0);

         
        name = g_strstrip(semicolon_tokens[0]);
        if (name && g_str_equal(name, media_type)) {
            result = true;
            g_strfreev(semicolon_tokens);
            goto cleanup;
        }
        g_strfreev(semicolon_tokens);
    }

 cleanup:
    g_strfreev(comma_tokens);
    return result;
}
