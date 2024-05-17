static void tv_details_cursor_changed(
                        GtkTreeView *tree_view,
                        gpointer     user_data_UNUSED)
{
     
    if (!GTK_IS_TREE_VIEW(tree_view))
        return;

    gchar *item_name = NULL;
    struct problem_item *item = get_current_problem_item_or_NULL(tree_view, &item_name);
    g_free(item_name);

     
    if (!item)
        return;

    gboolean editable = (item
                 
                && (item->flags & CD_FLAG_TXT)
                && !strchr(item->content, '\n')
    );

     
    g_object_set(G_OBJECT(g_tv_details_renderer_value),
                "editable", editable,
                NULL);
}
