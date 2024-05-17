user_extension_authentication_done (Daemon                *daemon,
                                    User                  *user,
                                    GDBusMethodInvocation *invocation,
                                    gpointer               user_data)
{
        GDBusInterfaceInfo *interface = user_data;
        const gchar *method_name;

        method_name = g_dbus_method_invocation_get_method_name (invocation);

        if (g_str_equal (method_name, "Get"))
                user_extension_get_property (user, daemon, interface, invocation);
        else if (g_str_equal (method_name, "GetAll"))
                user_extension_get_all_properties (user, daemon, interface, invocation);
        else if (g_str_equal (method_name, "Set"))
                user_extension_set_property (user, daemon, interface, invocation);
        else
                g_assert_not_reached ();
}
