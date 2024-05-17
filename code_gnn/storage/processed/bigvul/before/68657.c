_handle_conference(xmpp_stanza_t *const stanza)
{
    xmpp_stanza_t *xns_conference = xmpp_stanza_get_child_by_ns(stanza, STANZA_NS_CONFERENCE);

    const char *from = xmpp_stanza_get_from(stanza);
    if (!from) {
        log_warning("Message received with no from attribute, ignoring");
        return;
    }

    Jid *jidp = jid_create(from);
    if (!jidp) {
        return;
    }

    const char *room = xmpp_stanza_get_attribute(xns_conference, STANZA_ATTR_JID);
    if (!room) {
        jid_destroy(jidp);
        return;
    }

    const char *reason = xmpp_stanza_get_attribute(xns_conference, STANZA_ATTR_REASON);
    const char *password = xmpp_stanza_get_attribute(xns_conference, STANZA_ATTR_PASSWORD);

    sv_ev_room_invite(INVITE_DIRECT, jidp->barejid, room, reason, password);
    jid_destroy(jidp);
}