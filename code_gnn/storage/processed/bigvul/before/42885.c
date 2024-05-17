static void update_ls_details_checkboxes(const char *event_name)
{
    event_config_t *cfg = get_event_config(event_name);
    GHashTableIter iter;
    char *name;
    struct problem_item *item;
    g_hash_table_iter_init(&iter, g_cd);
    string_vector_ptr_t global_exclude = get_global_always_excluded_elements();
    while (g_hash_table_iter_next(&iter, (void**)&name, (void**)&item))
    {
         
        item->allowed_by_reporter = 1;
        if (global_exclude)
            item->allowed_by_reporter = !is_in_string_list(name, (const_string_vector_const_ptr_t)global_exclude);

        if (cfg)
        {
            if (is_in_comma_separated_list_of_glob_patterns(name, cfg->ec_exclude_items_always))
                item->allowed_by_reporter = 0;
            if ((item->flags & CD_FLAG_BIN) && cfg->ec_exclude_binary_items)
                item->allowed_by_reporter = 0;
        }

        item->default_by_reporter = item->allowed_by_reporter;
        if (cfg)
        {
            if (is_in_comma_separated_list_of_glob_patterns(name, cfg->ec_exclude_items_by_default))
                item->default_by_reporter = 0;
            if (is_in_comma_separated_list_of_glob_patterns(name, cfg->ec_include_items_by_default))
                item->allowed_by_reporter = item->default_by_reporter = 1;
        }

        item->required_by_reporter = 0;
        if (cfg)
        {
            if (is_in_comma_separated_list_of_glob_patterns(name, cfg->ec_requires_items))
                item->default_by_reporter = item->allowed_by_reporter = item->required_by_reporter = 1;
        }

        int cur_value;
        if (item->selected_by_user == 0)
            cur_value = item->default_by_reporter;
        else
            cur_value = !!(item->selected_by_user + 1);  


         
        GtkTreeIter iter;
        if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(g_ls_details), &iter))
        {
            do {
                gchar *item_name = NULL;
                gtk_tree_model_get(GTK_TREE_MODEL(g_ls_details), &iter,
                            DETAIL_COLUMN_NAME, &item_name,
                            -1);
                if (!item_name)  
                    continue;
                int differ = strcmp(name, item_name);
                g_free(item_name);
                if (differ)
                    continue;
                gtk_list_store_set(g_ls_details, &iter,
                        DETAIL_COLUMN_CHECKBOX, cur_value,
                        -1);
                break;
            } while (gtk_tree_model_iter_next(GTK_TREE_MODEL(g_ls_details), &iter));
        }
    }
}