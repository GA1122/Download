inject_handler_args(const gchar *actname, const gchar *origargs, const gchar *newargs) {
     
    GArray *rets = g_array_new(TRUE, FALSE, sizeof(gchar*));
     
    gchar *actdup = g_strdup(actname);
    g_array_append_val(rets, actdup);

    if ((g_strcmp0(actname, "spawn") == 0) ||
        (g_strcmp0(actname, "sh") == 0) ||
        (g_strcmp0(actname, "sync_spawn") == 0) ||
        (g_strcmp0(actname, "sync_sh") == 0) ||
        (g_strcmp0(actname, "talk_to_socket") == 0)) {
        guint i;
        GString *a = g_string_new("");
        gchar **spawnparts = split_quoted(origargs, FALSE);
        g_string_append_printf(a, "%s", spawnparts[0]);  
        if (newargs) g_string_append_printf(a, " %s", newargs);  

        for (i = 1; i < g_strv_length(spawnparts); i++)  
            if (spawnparts[i]) g_string_append_printf(a, " %s", spawnparts[i]);

        g_array_append_val(rets, a->str);
        g_string_free(a, FALSE);
        g_strfreev(spawnparts);
    } else {
        gchar *origdup = g_strdup(origargs);
        g_array_append_val(rets, origdup);
    }
    return (gchar**)g_array_free(rets, FALSE);
}
