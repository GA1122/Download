got_buffer_from_bus (FlatpakProxyClient *client, ProxySide *side, Buffer *buffer)
{
  if (client->authenticated && client->proxy->filter)
    {
      g_autoptr(Header) header = NULL;;
      GDBusMessage *rewritten;
      FlatpakPolicy policy;
      ExpectedReplyType expected_reply;

       

      header = parse_header (buffer, 0, client->serial_offset, client->hello_serial);
      if (header == NULL)
        {
          g_warning ("Invalid message header format");
          buffer_unref (buffer);
          side_closed (side);
          return;
        }

      if (!update_socket_messages (side, buffer, header))
        return;

      if (client->proxy->log_messages)
        print_incoming_header (header);

      if (header->has_reply_serial)
        {
          expected_reply = steal_expected_reply (get_other_side (side), header->reply_serial);

           
          if (expected_reply == EXPECTED_REPLY_NONE)
            {
              if (client->proxy->log_messages)
                g_print ("*Unexpected reply*\n");
              buffer_unref (buffer);
              return;
            }

          switch (expected_reply)
            {
            case EXPECTED_REPLY_HELLO:
               
              if (header->type == G_DBUS_MESSAGE_TYPE_METHOD_RETURN)
                {
                  g_autofree char *my_id = get_arg0_string (buffer);
                  flatpak_proxy_client_update_unique_id_policy (client, my_id, FLATPAK_POLICY_TALK);
                  break;
                }

            case EXPECTED_REPLY_REWRITE:
               

              rewritten = g_hash_table_lookup (client->rewrite_reply,
                                               GINT_TO_POINTER (header->reply_serial));

              if (client->proxy->log_messages)
                g_print ("*REWRITTEN*\n");

              g_dbus_message_set_serial (rewritten, header->serial);
              g_clear_pointer (&buffer, buffer_unref);
              buffer = message_to_buffer (rewritten);

              g_hash_table_remove (client->rewrite_reply,
                                   GINT_TO_POINTER (header->reply_serial));
              break;

            case EXPECTED_REPLY_FAKE_LIST_NAMES:
               

              queue_wildcard_initial_name_ops (client, header, buffer);

               
              if (client->proxy->log_messages)
                g_print ("*SKIPPED*\n");
              g_clear_pointer (&buffer, buffer_unref);

               
              start_reading (&client->client_side);
              break;

            case EXPECTED_REPLY_FAKE_GET_NAME_OWNER:
               
              {
                char *requested_name = g_hash_table_lookup (client->get_owner_reply, GINT_TO_POINTER (header->reply_serial));

                if (header->type == G_DBUS_MESSAGE_TYPE_METHOD_RETURN)
                  {
                    g_autofree char *owner = get_arg0_string (buffer);
                    flatpak_proxy_client_update_unique_id_policy_from_name (client, owner, requested_name);
                  }

                g_hash_table_remove (client->get_owner_reply, GINT_TO_POINTER (header->reply_serial));

                 
                if (client->proxy->log_messages)
                  g_print ("*SKIPPED*\n");
                g_clear_pointer (&buffer, buffer_unref);
                break;
              }

            case EXPECTED_REPLY_FILTER:
              if (client->proxy->log_messages)
                g_print ("*SKIPPED*\n");
              g_clear_pointer (&buffer, buffer_unref);
              break;

            case EXPECTED_REPLY_LIST_NAMES:
               
              if (header->type == G_DBUS_MESSAGE_TYPE_METHOD_RETURN)
                {
                  Buffer *filtered_buffer;

                  filtered_buffer = filter_names_list (client, buffer);
                  g_clear_pointer (&buffer, buffer_unref);
                  buffer = filtered_buffer;
                }

              break;

            case EXPECTED_REPLY_NORMAL:
              break;

            default:
              g_warning ("Unexpected expected reply type %d", expected_reply);
            }
        }
      else  
        {

           
          if (header->type == G_DBUS_MESSAGE_TYPE_METHOD_RETURN ||
              header->type == G_DBUS_MESSAGE_TYPE_ERROR)
            {
              if (client->proxy->log_messages)
                g_print ("*Invalid reply*\n");
              g_clear_pointer (&buffer, buffer_unref);
            }

           
          if (message_is_name_owner_changed (client, header))
            {
              if (should_filter_name_owner_changed (client, buffer))
                g_clear_pointer (&buffer, buffer_unref);
            }
        }

       
      if (header->type == G_DBUS_MESSAGE_TYPE_SIGNAL && header->destination == NULL)
        {
          policy = flatpak_proxy_client_get_policy (client, header->sender);
          if (policy < FLATPAK_POLICY_TALK)
            {
              if (client->proxy->log_messages)
                g_print ("*FILTERED IN*\n");
              g_clear_pointer (&buffer, buffer_unref);
            }
        }

       
      if (buffer && header->sender && header->sender[0] == ':')
        flatpak_proxy_client_update_unique_id_policy (client, header->sender, FLATPAK_POLICY_SEE);

      if (buffer && client_message_generates_reply (header))
        queue_expected_reply (side, header->serial, EXPECTED_REPLY_NORMAL);
    }

  if (buffer)
    queue_outgoing_buffer (&client->client_side, buffer);
}