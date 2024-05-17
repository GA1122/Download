static void on_failed_event(const char *event_name)
{
     
    if (strcmp(event_name, EMERGENCY_ANALYSIS_EVENT_NAME) == 0)
        return;

   add_warning(
_("Processing of the problem failed. This can have many reasons but there are three most common:\n"\
"\t▫ <b>network connection problems</b>\n"\
"\t▫ <b>corrupted problem data</b>\n"\
"\t▫ <b>invalid configuration</b>"
));

    if (!g_expert_mode)
    {
        add_warning(
_("If you want to update the configuration and try to report again, please open <b>Preferences</b> item\n"
"in the application menu and after applying the configuration changes click <b>Repeat</b> button."));
        gtk_widget_show(g_btn_repeat);
    }

    add_warning(
_("If you are sure that this problem is not caused by network problems neither by invalid configuration\n"
"and want to help us, please click on the upload button and provide all problem data for a deep analysis.\n"\
"<i>Before you do that, please consider the security risks. Problem data may contain sensitive information like\n"\
"passwords. The uploaded data are stored in a protected storage and only a limited number of persons can read them.</i>"));

    show_warnings();

    gtk_widget_show(g_btn_onfail);
}