static void create_details_treeview(void)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes(
                _("Include"), renderer,
                 
                "active", DETAIL_COLUMN_CHECKBOX,
                NULL);
    g_tv_details_col_checkbox = column;
    gtk_tree_view_append_column(g_tv_details, column);
     
    g_signal_connect(renderer, "toggled", G_CALLBACK(g_tv_details_checkbox_toggled), NULL);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(
                _("Name"), renderer,
                "text", DETAIL_COLUMN_NAME,
                NULL);
    gtk_tree_view_append_column(g_tv_details, column);
     
    gtk_tree_view_column_set_sort_column_id(column, DETAIL_COLUMN_NAME);

    g_tv_details_renderer_value = renderer = gtk_cell_renderer_text_new();
    g_signal_connect(renderer, "edited", G_CALLBACK(save_edited_one_liner), NULL);
    column = gtk_tree_view_column_new_with_attributes(
                _("Value"), renderer,
                "text", DETAIL_COLUMN_VALUE,
                NULL);
    gtk_tree_view_append_column(g_tv_details, column);
     
    gtk_tree_view_column_set_sort_column_id(column, DETAIL_COLUMN_VALUE);

     

    g_ls_details = gtk_list_store_new(DETAIL_NUM_COLUMNS, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model(g_tv_details, GTK_TREE_MODEL(g_ls_details));

    g_signal_connect(g_tv_details, "row-activated", G_CALLBACK(tv_details_row_activated), NULL);
    g_signal_connect(g_tv_details, "cursor-changed", G_CALLBACK(tv_details_cursor_changed), NULL);
     
}