cib_remote_new(const char *server, const char *user, const char *passwd, int port,
               gboolean encrypted)
{
    cib_remote_opaque_t *private = NULL;
    cib_t *cib = cib_new_variant();

    private = calloc(1, sizeof(cib_remote_opaque_t));

    cib->variant = cib_remote;
    cib->variant_opaque = private;

    if (server) {
        private->server = strdup(server);
    }

    if (user) {
        private->user = strdup(user);
    }

    if (passwd) {
        private->passwd = strdup(passwd);
    }

    private->port = port;
    private->command.encrypted = encrypted;
    private->callback.encrypted = encrypted;

     
    cib->delegate_fn = cib_remote_perform_op;
    cib->cmds->signon = cib_remote_signon;
    cib->cmds->signoff = cib_remote_signoff;
    cib->cmds->free = cib_remote_free;
    cib->cmds->inputfd = cib_remote_inputfd;

    cib->cmds->register_notification = cib_remote_register_notification;
    cib->cmds->set_connection_dnotify = cib_remote_set_connection_dnotify;

    return cib;
}