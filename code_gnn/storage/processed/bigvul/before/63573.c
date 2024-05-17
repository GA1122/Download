register_client_callback (SmsConn    conn,
                          SmPointer  manager_data,
                          char      *previous_id)
{
        GsmXSMPClient *client = manager_data;
        gboolean       handled;
        char          *id;

        g_debug ("GsmXSMPClient: Client '%s' received RegisterClient(%s)",
                 client->priv->description,
                 previous_id ? previous_id : "NULL");


         
        id = g_strdup (previous_id);

        handled = FALSE;
        g_signal_emit (client, signals[REGISTER_REQUEST], 0, &id, &handled);
        if (! handled) {
                g_debug ("GsmXSMPClient:  RegisterClient not handled!");
                g_free (id);
                free (previous_id);
                g_assert_not_reached ();
                return FALSE;
        }

        if (IS_STRING_EMPTY (id)) {
                g_debug ("GsmXSMPClient:   rejected: invalid previous_id");
                free (previous_id);
                return FALSE;
        }

        g_object_set (client, "startup-id", id, NULL);

        set_description (client);

        g_debug ("GsmXSMPClient: Sending RegisterClientReply to '%s'", client->priv->description);

        SmsRegisterClientReply (conn, id);

        if (IS_STRING_EMPTY (previous_id)) {
                 
                g_debug ("GsmXSMPClient: Sending initial SaveYourself");
                SmsSaveYourself (conn, SmSaveLocal, False, SmInteractStyleNone, False);
                client->priv->current_save_yourself = SmSaveLocal;
        }

        gsm_client_set_status (GSM_CLIENT (client), GSM_CLIENT_REGISTERED);

        g_free (id);
        free (previous_id);

        return TRUE;
}
