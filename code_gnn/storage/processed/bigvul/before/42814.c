static struct problem_item *get_current_problem_item_or_NULL(GtkTreeView *tree_view, gchar **pp_item_name)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreeSelection* selection = gtk_tree_view_get_selection(tree_view);

    if (selection == NULL)
        return NULL;

    if (!gtk_tree_selection_get_selected(selection, &model, &iter))
        return NULL;

    *pp_item_name = NULL;
    gtk_tree_model_get(model, &iter,
                DETAIL_COLUMN_NAME, pp_item_name,
                -1);
    if (!*pp_item_name)  
        return NULL;
    struct problem_item *item = problem_data_get_item_or_NULL(g_cd, *pp_item_name);
    return item;
}