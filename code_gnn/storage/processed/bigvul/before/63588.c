xsmp_query_end_session (GsmClient *client,
                        guint      flags,
                        GError   **error)
{
        gboolean allow_interact;
        int      save_type;

        if (GSM_XSMP_CLIENT (client)->priv->conn == NULL) {
                g_set_error (error,
                             GSM_CLIENT_ERROR,
                             GSM_CLIENT_ERROR_NOT_REGISTERED,
                             "Client is not registered");
                return FALSE;
        }

        allow_interact = !(flags & GSM_CLIENT_END_SESSION_FLAG_FORCEFUL);

         
        save_type = SmSaveGlobal;

        do_save_yourself (GSM_XSMP_CLIENT (client), save_type, allow_interact);
        return TRUE;
}
