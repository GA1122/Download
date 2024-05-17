static void on_btn_failed_cb(GtkButton *button)
{
     
    gtk_widget_hide(g_btn_repeat);
    terminate_event_chain(TERMINATE_NOFLAGS);

     
    gtk_expander_set_expanded(g_exp_report_log, TRUE);

    clear_warnings();
    update_ls_details_checkboxes(EMERGENCY_ANALYSIS_EVENT_NAME);
    start_event_run(EMERGENCY_ANALYSIS_EVENT_NAME);

     
    gtk_widget_hide(GTK_WIDGET(button));
}
