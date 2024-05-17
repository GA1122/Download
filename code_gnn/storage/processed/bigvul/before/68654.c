_handel_muc_user(xmpp_stanza_t *const stanza)
{
    xmpp_ctx_t *ctx = connection_get_ctx();
    xmpp_stanza_t *xns_muc_user = xmpp_stanza_get_child_by_ns(stanza, STANZA_NS_MUC_USER);
    const char *room = xmpp_stanza_get_from(stanza);

    if (!room) {
        log_warning("Message received with no from attribute, ignoring");
        return;
    }

    xmpp_stanza_t *invite = xmpp_stanza_get_child_by_name(xns_muc_user, STANZA_NAME_INVITE);
    if (!invite) {
        return;
    }

    const char *invitor_jid = xmpp_stanza_get_from(invite);
    if (!invitor_jid) {
        log_warning("Chat room invite received with no from attribute");
        return;
    }

    Jid *jidp = jid_create(invitor_jid);
    if (!jidp) {
        return;
    }
    char *invitor = jidp->barejid;

    char *reason = NULL;
    xmpp_stanza_t *reason_st = xmpp_stanza_get_child_by_name(invite, STANZA_NAME_REASON);
    if (reason_st) {
        reason = xmpp_stanza_get_text(reason_st);
    }

    char *password = NULL;
    xmpp_stanza_t *password_st = xmpp_stanza_get_child_by_name(xns_muc_user, STANZA_NAME_PASSWORD);
    if (password_st) {
        password = xmpp_stanza_get_text(password_st);
    }

    sv_ev_room_invite(INVITE_MEDIATED, invitor, room, reason, password);
    jid_destroy(jidp);
    if (reason) {
        xmpp_free(ctx, reason);
    }
    if (password) {
        xmpp_free(ctx, password);
    }
}