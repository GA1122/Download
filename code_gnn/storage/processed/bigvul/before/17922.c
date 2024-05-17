static void vnc_client_cache_auth(VncState *client)
{
#if defined(CONFIG_VNC_TLS) || defined(CONFIG_VNC_SASL)
    QDict *qdict;
#endif

    if (!client->info) {
        return;
    }

#if defined(CONFIG_VNC_TLS) || defined(CONFIG_VNC_SASL)
    qdict = qobject_to_qdict(client->info);
#endif

#ifdef CONFIG_VNC_TLS
    if (client->tls.session &&
        client->tls.dname) {
        qdict_put(qdict, "x509_dname", qstring_from_str(client->tls.dname));
    }
#endif
#ifdef CONFIG_VNC_SASL
    if (client->sasl.conn &&
        client->sasl.username) {
        qdict_put(qdict, "sasl_username",
                  qstring_from_str(client->sasl.username));
    }
#endif
}
