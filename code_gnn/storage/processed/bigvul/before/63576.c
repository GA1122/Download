save_yourself_request_callback (SmsConn   conn,
                                SmPointer manager_data,
                                int       save_type,
                                Bool      shutdown,
                                int       interact_style,
                                Bool      fast,
                                Bool      global)
{
        GsmXSMPClient *client = manager_data;

        g_debug ("GsmXSMPClient: Client '%s' received SaveYourselfRequest(%s, %s, %s, %s, %s)",
                 client->priv->description,
                 save_type == SmSaveLocal ? "SmSaveLocal" :
                 save_type == SmSaveGlobal ? "SmSaveGlobal" : "SmSaveBoth",
                 shutdown ? "Shutdown" : "!Shutdown",
                 interact_style == SmInteractStyleAny ? "SmInteractStyleAny" :
                 interact_style == SmInteractStyleErrors ? "SmInteractStyleErrors" :
                 "SmInteractStyleNone", fast ? "Fast" : "!Fast",
                 global ? "Global" : "!Global");

         

        if (shutdown && global) {
                g_debug ("GsmXSMPClient:   initiating shutdown");
                g_signal_emit (client, signals[LOGOUT_REQUEST], 0, !fast);
        } else if (!shutdown && !global) {
                g_debug ("GsmXSMPClient:   initiating checkpoint");
                do_save_yourself (client, SmSaveLocal, TRUE);
        } else {
                g_debug ("GsmXSMPClient:   ignoring");
        }
}
