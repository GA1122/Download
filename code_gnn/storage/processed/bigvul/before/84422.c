queue_wildcard_initial_name_ops (FlatpakProxyClient *client, Header *header, Buffer *buffer)
{
  GDBusMessage *decoded_message = g_dbus_message_new_from_blob (buffer->data, buffer->size, 0, NULL);
  GVariant *body, *arg0;

  if (decoded_message != NULL &&
      header->type == G_DBUS_MESSAGE_TYPE_METHOD_RETURN &&
      (body = g_dbus_message_get_body (decoded_message)) != NULL &&
      (arg0 = g_variant_get_child_value (body, 0)) != NULL &&
      g_variant_is_of_type (arg0, G_VARIANT_TYPE_STRING_ARRAY))
    {
      const gchar **names = g_variant_get_strv (arg0, NULL);
      int i;

       
      for (i = 0; names[i] != NULL; i++)
        {
          const char *name = names[i];

          if (name[0] != ':' &&
              flatpak_proxy_get_wildcard_policy (client->proxy, name) != FLATPAK_POLICY_NONE)
            {
               
              GDBusMessage *message = g_dbus_message_new_method_call ("org.freedesktop.DBus", "/", "org.freedesktop.DBus", "GetNameOwner");
              g_dbus_message_set_body (message, g_variant_new ("(s)", name));
              queue_fake_message (client, message, EXPECTED_REPLY_FAKE_GET_NAME_OWNER);
              g_hash_table_replace (client->get_owner_reply, GINT_TO_POINTER (client->last_serial), g_strdup (name));

              if (client->proxy->log_messages)
                g_print ("C%d: -> org.freedesktop.DBus fake GetNameOwner for %s\n", client->last_serial, name);
            }
        }
      g_free (names);
    }

  g_object_unref (decoded_message);
}