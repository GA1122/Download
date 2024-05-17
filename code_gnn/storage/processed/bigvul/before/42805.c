static void delete_item(GtkTreeView *treeview)
{
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    if (selection)
    {
        GtkTreeIter iter;
        GtkTreeModel *store = gtk_tree_view_get_model(treeview);
        if (gtk_tree_selection_get_selected(selection, &store, &iter) == TRUE)
        {
            GValue d_item_name = { 0 };
            gtk_tree_model_get_value(store, &iter, DETAIL_COLUMN_NAME, &d_item_name);
            const char *item_name = g_value_get_string(&d_item_name);
            if (item_name)
            {
                struct problem_item *item = problem_data_get_item_or_NULL(g_cd, item_name);
                if (item->flags & CD_FLAG_ISEDITABLE)
                {

                    struct dump_dir *dd = wizard_open_directory_for_writing(g_dump_dir_name);
                    if (dd)
                    {
                        char *filename = concat_path_file(g_dump_dir_name, item_name);
                        unlink(filename);
                        free(filename);
                        dd_close(dd);
                        g_hash_table_remove(g_cd, item_name);
                        gtk_list_store_remove(g_ls_details, &iter);
                    }

                }
            }
        }
    }
}