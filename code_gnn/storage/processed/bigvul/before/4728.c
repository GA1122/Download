user_extension_get_all_properties (User                  *user,
                                   Daemon                *daemon,
                                   GDBusInterfaceInfo    *interface,
                                   GDBusMethodInvocation *invocation)
{
        GVariantBuilder builder;
        gint i;

        g_variant_builder_init (&builder, G_VARIANT_TYPE_VARDICT);
        for (i = 0; interface->properties && interface->properties[i]; i++) {
                GDBusPropertyInfo *property = interface->properties[i];
                g_autoptr(GVariant) value = NULL;

                value = user_extension_get_value (user, interface, property);

                if (value)
                        g_variant_builder_add (&builder, "{sv}", property->name, value);
        }

        g_dbus_method_invocation_return_value (invocation, g_variant_new ("(a{sv})", &builder));
}
