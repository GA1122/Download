static void on_btn_add_file(GtkButton *button)
{
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
            "Attach File",
            GTK_WINDOW(g_wnd_assistant),
            GTK_FILE_CHOOSER_ACTION_OPEN,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            _("_Open"), GTK_RESPONSE_ACCEPT,
            NULL
    );
    char *filename = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_widget_destroy(dialog);

    if (filename)
    {
        char *basename = strrchr(filename, '/');
        if (!basename)   
            goto free_and_ret;
        basename++;

         

        char *message = NULL;

        struct stat statbuf;
        if (stat(filename, &statbuf) != 0 || !S_ISREG(statbuf.st_mode))
        {
            message = xasprintf(_("'%s' is not an ordinary file"), filename);
            goto show_msgbox;
        }

        struct problem_item *item = problem_data_get_item_or_NULL(g_cd, basename);
        if (!item || (item->flags & CD_FLAG_ISEDITABLE))
        {
            struct dump_dir *dd = wizard_open_directory_for_writing(g_dump_dir_name);
            if (dd)
            {
                dd_close(dd);
                char *new_name = concat_path_file(g_dump_dir_name, basename);
                if (strcmp(filename, new_name) == 0)
                {
                    message = xstrdup(_("You are trying to copy a file onto itself"));
                }
                else
                {
                    off_t r = copy_file(filename, new_name, 0666);
                    if (r < 0)
                    {
                        message = xasprintf(_("Can't copy '%s': %s"), filename, strerror(errno));
                        unlink(new_name);
                    }
                    if (!message)
                    {
                        problem_data_reload_from_dump_dir();
                        update_gui_state_from_problem_data(  0);
                         
                        update_ls_details_checkboxes(g_event_selected);
                    }
                }
                free(new_name);
            }
        }
        else
            message = xasprintf(_("Item '%s' already exists and is not modifiable"), basename);

        if (message)
        {
 show_msgbox: ;
            GtkWidget *dlg = gtk_message_dialog_new(GTK_WINDOW(g_wnd_assistant),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_WARNING,
                GTK_BUTTONS_CLOSE,
                message);
            free(message);
            gtk_window_set_transient_for(GTK_WINDOW(dlg), GTK_WINDOW(g_wnd_assistant));
            gtk_dialog_run(GTK_DIALOG(dlg));
            gtk_widget_destroy(dlg);
        }
 free_and_ret:
        g_free(filename);
    }
}