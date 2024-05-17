_handle_captcha(xmpp_stanza_t *const stanza)
{
    xmpp_ctx_t *ctx = connection_get_ctx();
    const char *from = xmpp_stanza_get_from(stanza);

    if (!from) {
        log_warning("Message received with no from attribute, ignoring");
        return;
    }

    char *message = xmpp_message_get_body(stanza);
    if (!message) {
        return;
    }

    sv_ev_room_broadcast(from, message);
    xmpp_free(ctx, message);
}