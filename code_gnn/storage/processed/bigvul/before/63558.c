delete_property (GsmXSMPClient *client,
                 const char    *name)
{
        int     index;
        SmProp *prop;

        prop = find_property (client, name, &index);
        if (!prop) {
                return;
        }

#if 0
         
        if (!strcmp (name, SmDiscardCommand)) {
                gsm_client_run_discard (GSM_CLIENT (client));
        }
#endif

        g_ptr_array_remove_index_fast (client->priv->props, index);
        SmFreeProperty (prop);
}
