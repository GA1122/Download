destroy_server(struct server *server) {
    if (server->method) ss_free(server->method);
    if (server->plugin) ss_free(server->plugin);
    if (server->plugin_opts) ss_free(server->plugin_opts);
    if (server->mode) ss_free(server->mode);
 }
