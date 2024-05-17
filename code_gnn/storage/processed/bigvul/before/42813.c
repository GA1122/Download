static void g_tv_details_checkbox_toggled(
                        GtkCellRendererToggle *cell_renderer_UNUSED,
                        gchar    *tree_path,
                        gpointer  user_data_UNUSED)
{
    GtkTreeIter iter;
    if (!gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(g_ls_details), &iter, tree_path))
        return;

    gchar *item_name = NULL;
    gtk_tree_model_get(GTK_TREE_MODEL(g_ls_details), &iter,
                DETAIL_COLUMN_NAME, &item_name,
                -1);
    if (!item_name)  
        return;
    struct problem_item *item = problem_data_get_item_or_NULL(g_cd, item_name);
    g_free(item_name);
    if (!item)  
        return;

    int cur_value;
    if (item->selected_by_user == 0)
        cur_value = item->default_by_reporter;
    else
        cur_value = !!(item->selected_by_user + 1);  
    if (item->allowed_by_reporter && !item->required_by_reporter)
    {
        cur_value = !cur_value;
        item->selected_by_user = cur_value * 2 - 1;  
        gtk_list_store_set(g_ls_details, &iter,
                DETAIL_COLUMN_CHECKBOX, cur_value,
                -1);
    }
}