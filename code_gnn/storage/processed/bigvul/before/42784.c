static event_gui_data_t *add_event_buttons(GtkBox *box,
                GList **p_event_list,
                char *event_name,
                GCallback func)
{
    gtk_container_foreach(GTK_CONTAINER(box), &remove_child_widget, NULL);
    g_list_foreach(*p_event_list, (GFunc)free_event_gui_data_t, NULL);
    g_list_free(*p_event_list);
    *p_event_list = NULL;

    g_black_event_count = 0;

    if (!event_name || !event_name[0])
    {
        GtkWidget *lbl = gtk_label_new(_("No reporting targets are defined for this problem. Check configuration in /etc/libreport/*"));
#if ((GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 13) || (GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION == 13 && GTK_MICRO_VERSION < 5))
        gtk_misc_set_alignment(GTK_MISC(lbl),   0.0,   0.0);
#else
        gtk_widget_set_halign (lbl, GTK_ALIGN_START);
        gtk_widget_set_valign (lbl, GTK_ALIGN_END);
#endif
        make_label_autowrap_on_resize(GTK_LABEL(lbl));
        gtk_box_pack_start(box, lbl,   true,   false,   0);
        return NULL;
    }

    event_gui_data_t *first_button = NULL;
    event_gui_data_t *active_button = NULL;
    while (1)
    {
        if (!event_name || !event_name[0])
            break;

        char *event_name_end = strchr(event_name, '\n');
        *event_name_end = '\0';

        event_config_t *cfg = get_event_config(event_name);

         
         
        const char *event_screen_name = event_name;
        const char *event_description = NULL;
        char *tmp_description = NULL;
        bool red_choice = false;
        bool green_choice = false;
        if (cfg)
        {
             
            if (ec_get_screen_name(cfg))
                event_screen_name = ec_get_screen_name(cfg);
            event_description = ec_get_description(cfg);

            char *missing = missing_items_in_comma_list(cfg->ec_requires_items);
            if (missing)
            {
                red_choice = true;
                event_description = tmp_description = xasprintf(_("(requires: %s)"), missing);
                free(missing);
            }
            else
            if (cfg->ec_creates_items)
            {
                if (problem_data_get_item_or_NULL(g_cd, cfg->ec_creates_items))
                {
                    char *missing = missing_items_in_comma_list(cfg->ec_creates_items);
                    if (missing)
                        free(missing);
                    else
                    {
                        green_choice = true;
                        event_description = tmp_description = xasprintf(_("(not needed, data already exist: %s)"), cfg->ec_creates_items);
                    }
                }
            }
        }
        if (!green_choice && !red_choice)
            g_black_event_count++;

        char *event_label = xasprintf("%s%s%s",
                        event_screen_name,
                        (event_description ? " - " : ""),
                        event_description ? event_description : ""
        );
        free(tmp_description);

        GtkWidget *button = gtk_radio_button_new_with_label_from_widget(
                        (first_button ? GTK_RADIO_BUTTON(first_button->toggle_button) : NULL),
                        event_label
                );
        free(event_label);

        if (green_choice || red_choice)
        {
            GtkWidget *child = gtk_bin_get_child(GTK_BIN(button));
            if (child)
            {
                static const GdkRGBA red = {
                    .red   = 1.0,
                    .green = 0.0,
                    .blue  = 0.0,
                    .alpha = 1.0,
                };
                static const GdkRGBA green = {
                    .red   = 0.0,
                    .green = 0.5,
                    .blue  = 0.0,
                    .alpha = 1.0,
                };
                const GdkRGBA *color = (green_choice ? &green : &red);
                gtk_widget_override_color(child, GTK_STATE_FLAG_NORMAL, color);
            }
        }

        if (func)
            g_signal_connect(G_OBJECT(button), "toggled", func, xstrdup(event_name));

        if (cfg && ec_get_long_desc(cfg))
            gtk_widget_set_tooltip_text(button, ec_get_long_desc(cfg));

        event_gui_data_t *event_gui_data = new_event_gui_data_t();
        event_gui_data->event_name = xstrdup(event_name);
        event_gui_data->toggle_button = GTK_TOGGLE_BUTTON(button);
        *p_event_list = g_list_append(*p_event_list, event_gui_data);

        if (!first_button)
            first_button = event_gui_data;

        if (!green_choice && !red_choice && !active_button)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), true);
            active_button = event_gui_data;
        }

        *event_name_end = '\n';
        event_name = event_name_end + 1;

        gtk_box_pack_start(box, button,   false,   false,   0);
        gtk_widget_show_all(GTK_WIDGET(button));
        wrap_all_labels(button);
         
         
    }
    gtk_widget_show_all(GTK_WIDGET(box));

    return active_button;
}