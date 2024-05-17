void create_assistant(GtkApplication *app, bool expert_mode)
{
    g_loaded_texts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    g_expert_mode = expert_mode;

    g_monospace_font = pango_font_description_from_string("monospace");

    g_assistant = GTK_NOTEBOOK(gtk_notebook_new());

     
    gtk_notebook_set_show_tabs(g_assistant, (g_verbose != 0 && g_expert_mode));

    g_btn_close = gtk_button_new_with_mnemonic(_("_Close"));
    gtk_button_set_image(GTK_BUTTON(g_btn_close), gtk_image_new_from_icon_name("window-close-symbolic", GTK_ICON_SIZE_BUTTON));
    g_btn_stop = gtk_button_new_with_mnemonic(_("_Stop"));
    gtk_button_set_image(GTK_BUTTON(g_btn_stop), gtk_image_new_from_icon_name("process-stop-symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_widget_set_no_show_all(g_btn_stop, true);  
    g_btn_onfail = gtk_button_new_with_label(_("Upload for analysis"));
    gtk_button_set_image(GTK_BUTTON(g_btn_onfail), gtk_image_new_from_icon_name("go-up-symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_widget_set_no_show_all(g_btn_onfail, true);  
    g_btn_repeat = gtk_button_new_with_label(_("Repeat"));
    gtk_widget_set_no_show_all(g_btn_repeat, true);  
    g_btn_next = gtk_button_new_with_mnemonic(_("_Forward"));
    gtk_button_set_image(GTK_BUTTON(g_btn_next), gtk_image_new_from_icon_name("go-next-symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_widget_set_no_show_all(g_btn_next, true);  
    g_btn_detail = gtk_button_new_with_mnemonic(_("Details"));
    gtk_widget_set_no_show_all(g_btn_detail, true);  

    g_box_buttons = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_box_pack_start(g_box_buttons, g_btn_close, false, false, 5);
    gtk_box_pack_start(g_box_buttons, g_btn_stop, false, false, 5);
    gtk_box_pack_start(g_box_buttons, g_btn_onfail, false, false, 5);
    gtk_box_pack_start(g_box_buttons, g_btn_repeat, false, false, 5);
     
#if ((GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 13) || (GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION == 13 && GTK_MICRO_VERSION < 5))
    GtkWidget *w = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
    gtk_box_pack_start(g_box_buttons, w, true, true, 5);
    gtk_box_pack_start(g_box_buttons, g_btn_detail, false, false, 5);
    gtk_box_pack_start(g_box_buttons, g_btn_next, false, false, 5);
#else
    gtk_widget_set_valign(GTK_WIDGET(g_btn_next), GTK_ALIGN_END);
    gtk_box_pack_end(g_box_buttons, g_btn_next, false, false, 5);
    gtk_box_pack_end(g_box_buttons, g_btn_detail, false, false, 5);
#endif

    {    
        g_box_warning_labels = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
        gtk_widget_set_visible(GTK_WIDGET(g_box_warning_labels), TRUE);

        GtkBox *vbox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
        gtk_widget_set_visible(GTK_WIDGET(vbox), TRUE);
        gtk_box_pack_start(vbox, GTK_WIDGET(g_box_warning_labels), false, false, 5);

        GtkWidget *image = gtk_image_new_from_icon_name("dialog-warning-symbolic", GTK_ICON_SIZE_DIALOG);
        gtk_widget_set_visible(image, TRUE);

        g_widget_warnings_area = GTK_WIDGET(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
        gtk_widget_set_visible(g_widget_warnings_area, FALSE);
        gtk_widget_set_no_show_all(g_widget_warnings_area, TRUE);

#if ((GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 13) || (GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION == 13 && GTK_MICRO_VERSION < 5))
        GtkWidget *alignment_left = gtk_alignment_new(0.5,0.5,1,1);
        gtk_widget_set_visible(alignment_left, TRUE);
        gtk_box_pack_start(GTK_BOX(g_widget_warnings_area), alignment_left, true, false, 0);
#else
        gtk_widget_set_valign(GTK_WIDGET(image), GTK_ALIGN_CENTER);
        gtk_widget_set_valign(GTK_WIDGET(vbox), GTK_ALIGN_CENTER);
#endif

        gtk_box_pack_start(GTK_BOX(g_widget_warnings_area), image, false, false, 5);
        gtk_box_pack_start(GTK_BOX(g_widget_warnings_area), GTK_WIDGET(vbox), false, false, 0);

#if ((GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 13) || (GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION == 13 && GTK_MICRO_VERSION < 5))
        GtkWidget *alignment_right = gtk_alignment_new(0.5,0.5,1,1);
        gtk_widget_set_visible(alignment_right, TRUE);
        gtk_box_pack_start(GTK_BOX(g_widget_warnings_area), alignment_right, true, false, 0);
#endif
    }    

    g_box_assistant = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_box_pack_start(g_box_assistant, GTK_WIDGET(g_assistant), true, true, 0);

    gtk_box_pack_start(g_box_assistant, GTK_WIDGET(g_widget_warnings_area), false, false, 0);
    gtk_box_pack_start(g_box_assistant, GTK_WIDGET(g_box_buttons), false, false, 5);

    gtk_widget_show_all(GTK_WIDGET(g_box_buttons));
    gtk_widget_hide(g_btn_stop);
    gtk_widget_hide(g_btn_onfail);
    gtk_widget_hide(g_btn_repeat);
    gtk_widget_show(g_btn_next);

    g_wnd_assistant = GTK_WINDOW(gtk_application_window_new(app));
    gtk_container_add(GTK_CONTAINER(g_wnd_assistant), GTK_WIDGET(g_box_assistant));

    gtk_window_set_default_size(g_wnd_assistant, DEFAULT_WIDTH, DEFAULT_HEIGHT);
     
    gtk_window_set_default_icon_name("abrt");

    init_pages();

    add_pages();

    create_details_treeview();

    ProblemDetailsWidget *details = problem_details_widget_new(g_cd);
    gtk_container_add(GTK_CONTAINER(g_container_details1), GTK_WIDGET(details));

    g_signal_connect(g_btn_close, "clicked", G_CALLBACK(assistant_quit_cb), NULL);
    g_signal_connect(g_btn_stop, "clicked", G_CALLBACK(on_btn_cancel_event), NULL);
    g_signal_connect(g_btn_onfail, "clicked", G_CALLBACK(on_btn_failed_cb), NULL);
    g_signal_connect(g_btn_repeat, "clicked", G_CALLBACK(on_btn_repeat_cb), NULL);
    g_signal_connect(g_btn_next, "clicked", G_CALLBACK(on_next_btn_cb), NULL);

    g_signal_connect(g_wnd_assistant, "destroy", G_CALLBACK(assistant_quit_cb), NULL);
    g_signal_connect(g_assistant, "switch-page", G_CALLBACK(on_page_prepare), NULL);

    g_signal_connect(g_tb_approve_bt, "toggled", G_CALLBACK(on_bt_approve_toggle), NULL);
    g_signal_connect(gtk_text_view_get_buffer(g_tv_comment), "changed", G_CALLBACK(on_comment_changed), NULL);

    g_signal_connect(g_btn_add_file, "clicked", G_CALLBACK(on_btn_add_file), NULL);
    g_signal_connect(g_btn_detail, "clicked", G_CALLBACK(on_btn_detail), NULL);

    if (is_screencast_available()) {
         
        g_signal_connect(g_btn_startcast, "activate-link", G_CALLBACK(on_btn_startcast), NULL);
    }
    else {
        gtk_widget_set_sensitive(GTK_WIDGET(g_btn_startcast), false);
        gtk_widget_set_tooltip_markup(GTK_WIDGET(g_btn_startcast),
          _("In order to enable the built-in screencasting "
            "functionality the package fros-recordmydesktop has to be installed. "
            "Please run the following command if you want to install it."
            "\n\n"
            "<b>su -c \"yum install fros-recordmydesktop\"</b>"
            ));
    }


    g_signal_connect(g_search_entry_bt, "changed", G_CALLBACK(search_timeout), NULL);

    g_signal_connect (g_tv_event_log, "key-press-event", G_CALLBACK (key_press_event), NULL);
    g_signal_connect (g_tv_event_log, "event-after", G_CALLBACK (event_after), NULL);
    g_signal_connect (g_tv_event_log, "motion-notify-event", G_CALLBACK (motion_notify_event), NULL);
    g_signal_connect (g_tv_event_log, "visibility-notify-event", G_CALLBACK (visibility_notify_event), NULL);
    g_signal_connect (gtk_text_view_get_buffer(g_tv_event_log), "changed", G_CALLBACK (on_log_changed), NULL);

    hand_cursor = gdk_cursor_new (GDK_HAND2);
    regular_cursor = gdk_cursor_new (GDK_XTERM);

     
    on_page_prepare(g_assistant, gtk_notebook_get_nth_page(g_assistant, 0), NULL);
}