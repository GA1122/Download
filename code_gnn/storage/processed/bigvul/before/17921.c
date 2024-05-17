static void vnc_client_cache_addr(VncState *client)
{
    QDict *qdict;

    qdict = qdict_new();
    if (vnc_qdict_remote_addr(qdict, client->csock) < 0) {
        QDECREF(qdict);
         
        return;
    }

    client->info = QOBJECT(qdict);
}
