user_extension_set_property (User                  *user,
                             Daemon                *daemon,
                             GDBusInterfaceInfo    *interface,
                             GDBusMethodInvocation *invocation)
{
        const GDBusPropertyInfo *property = g_dbus_method_invocation_get_property_info (invocation);
        g_autoptr(GVariant) value = NULL;
        g_autofree gchar *printed = NULL;
        g_autofree gchar *prev = NULL;

        g_variant_get_child (g_dbus_method_invocation_get_parameters (invocation), 2, "v", &value);

         
        printed = g_variant_print (value, FALSE);

         
        prev = g_key_file_get_value (user->keyfile, interface->name, property->name, NULL);

        if (!prev || !g_str_equal (printed, prev)) {
                g_key_file_set_value (user->keyfile, interface->name, property->name, printed);

                 
                g_dbus_connection_emit_signal (g_dbus_method_invocation_get_connection (invocation),
                                               NULL,  
                                               g_dbus_method_invocation_get_object_path (invocation),
                                               "org.freedesktop.DBus.Properties", "PropertiesChanged",
                                               g_variant_new_parsed ("( %s, %a{sv}, [ %s ] )",
                                                                     interface->name, NULL, property->name),
                                               NULL);

                accounts_user_emit_changed (ACCOUNTS_USER (user));
                save_extra_data (user);
        }

        g_dbus_method_invocation_return_value (invocation, g_variant_new ("()"));
}
