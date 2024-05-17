static GtkBuilder *make_builder()
{
    GError *error = NULL;
    GtkBuilder *builder = gtk_builder_new();
    if (!g_glade_file)
    {
         
        gtk_builder_add_objects_from_string(builder,
                WIZARD_GLADE_CONTENTS, sizeof(WIZARD_GLADE_CONTENTS) - 1,
                (gchar**)misc_widgets,
                &error);
        if (error != NULL)
            error_msg_and_die("Error loading glade data: %s", error->message);
         
        gtk_builder_add_objects_from_string(builder,
                WIZARD_GLADE_CONTENTS, sizeof(WIZARD_GLADE_CONTENTS) - 1,
                (gchar**)page_names,
                &error);
        if (error != NULL)
            error_msg_and_die("Error loading glade data: %s", error->message);
    }
    else
    {
         
         
        gtk_builder_add_objects_from_file(builder, g_glade_file, (gchar**)misc_widgets, &error);
        if (error != NULL)
            error_msg_and_die("Can't load %s: %s", g_glade_file, error->message);
        gtk_builder_add_objects_from_file(builder, g_glade_file, (gchar**)page_names, &error);
        if (error != NULL)
            error_msg_and_die("Can't load %s: %s", g_glade_file, error->message);
    }

    return builder;
}
