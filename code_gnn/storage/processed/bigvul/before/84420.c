queue_initial_name_ops (FlatpakProxyClient *client)
{
  GHashTableIter iter;
  gpointer key, value;
  gboolean has_wildcards = FALSE;

  g_hash_table_iter_init (&iter, client->proxy->policy);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      const char *name = key;
      GDBusMessage *message;
      GVariant *match;

      if (strcmp (name, "org.freedesktop.DBus") == 0)
        continue;

       
      message = g_dbus_message_new_method_call ("org.freedesktop.DBus", "/", "org.freedesktop.DBus", "AddMatch");
      match = g_variant_new_printf ("type='signal',sender='org.freedesktop.DBus',interface='org.freedesktop.DBus',member='NameOwnerChanged',arg0='%s'", name);
      g_dbus_message_set_body (message, g_variant_new_tuple (&match, 1));
      queue_fake_message (client, message, EXPECTED_REPLY_FILTER);

      if (client->proxy->log_messages)
        g_print ("C%d: -> org.freedesktop.DBus fake AddMatch for %s\n", client->last_serial, name);

       
      message = g_dbus_message_new_method_call ("org.freedesktop.DBus", "/", "org.freedesktop.DBus", "GetNameOwner");
      g_dbus_message_set_body (message, g_variant_new ("(s)", name));
      queue_fake_message (client, message, EXPECTED_REPLY_FAKE_GET_NAME_OWNER);
      g_hash_table_replace (client->get_owner_reply, GINT_TO_POINTER (client->last_serial), g_strdup (name));

      if (client->proxy->log_messages)
        g_print ("C%d: -> org.freedesktop.DBus fake GetNameOwner for %s\n", client->last_serial, name);
    }

   
  g_hash_table_iter_init (&iter, client->proxy->wildcard_policy);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      const char *name = key;
      GDBusMessage *message;
      GVariant *match;

      has_wildcards = TRUE;

       
      message = g_dbus_message_new_method_call ("org.freedesktop.DBus", "/", "org.freedesktop.DBus", "AddMatch");
      match = g_variant_new_printf ("type='signal',sender='org.freedesktop.DBus',interface='org.freedesktop.DBus',member='NameOwnerChanged',arg0namespace='%s'", name);
      g_dbus_message_set_body (message, g_variant_new_tuple (&match, 1));
      queue_fake_message (client, message, EXPECTED_REPLY_FILTER);

      if (client->proxy->log_messages)
        g_print ("C%d: -> org.freedesktop.DBus fake AddMatch for %s.*\n", client->last_serial, name);
    }

  if (has_wildcards)
    {
      GDBusMessage *message;

       
      message = g_dbus_message_new_method_call ("org.freedesktop.DBus", "/", "org.freedesktop.DBus", "ListNames");
      g_dbus_message_set_body (message, g_variant_new ("()"));
      queue_fake_message (client, message, EXPECTED_REPLY_FAKE_LIST_NAMES);

      if (client->proxy->log_messages)
        g_print ("C%d: -> org.freedesktop.DBus fake ListNames\n", client->last_serial);

       
      stop_reading (&client->client_side);
    }
}
