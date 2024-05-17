static void set_excluded_envvar(void)
{
    struct strbuf *item_list = strbuf_new();
    const char *fmt = "%s";

    GtkTreeIter iter;
    if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(g_ls_details), &iter))
    {
        do {
            gchar *item_name = NULL;
            gboolean checked = 0;
            gtk_tree_model_get(GTK_TREE_MODEL(g_ls_details), &iter,
                    DETAIL_COLUMN_NAME, &item_name,
                    DETAIL_COLUMN_CHECKBOX, &checked,
                    -1);
            if (!item_name)  
                continue;
            if (!checked)
            {
                strbuf_append_strf(item_list, fmt, item_name);
                fmt = ",%s";
            }
            g_free(item_name);
        } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(g_ls_details), &iter));
    }
    char *var = strbuf_free_nobuf(item_list);
    if (var)
    {
        xsetenv("EXCLUDE_FROM_REPORT", var);
        free(var);
    }
    else
        unsetenv("EXCLUDE_FROM_REPORT");
}