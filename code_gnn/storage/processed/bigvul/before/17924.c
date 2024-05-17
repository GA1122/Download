static void vnc_qmp_event(VncState *vs, MonitorEvent event)
{
    QDict *server;
    QObject *data;

    if (!vs->info) {
        return;
    }

    server = qdict_new();
    if (vnc_server_info_put(server) < 0) {
        QDECREF(server);
        return;
    }

    data = qobject_from_jsonf("{ 'client': %p, 'server': %p }",
                              vs->info, QOBJECT(server));

    monitor_protocol_event(event, data);

    qobject_incref(vs->info);
    qobject_decref(data);
}
