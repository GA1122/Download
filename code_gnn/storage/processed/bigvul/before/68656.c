_handle_chat(xmpp_stanza_t *const stanza)
{
    const char *type = xmpp_stanza_get_type(stanza);
    if (!(g_strcmp0(type, "chat") == 0 || type == NULL)) {
        return;
    }

    gboolean res = _handle_carbons(stanza);
    if (res) {
        return;
    }

    xmpp_stanza_t *conf = xmpp_stanza_get_child_by_ns(stanza, STANZA_NS_CONFERENCE);
    xmpp_stanza_t *captcha = xmpp_stanza_get_child_by_ns(stanza, STANZA_NS_CAPTCHA);
    if (conf || captcha) {
        return;
    }

    xmpp_stanza_t *mucuser = xmpp_stanza_get_child_by_ns(stanza, STANZA_NS_MUC_USER);
    xmpp_stanza_t *body = xmpp_stanza_get_child_by_name(stanza, STANZA_NAME_BODY);
    if (mucuser && body == NULL) {
        return;
    }

    const gchar *from = xmpp_stanza_get_from(stanza);
    Jid *jid = jid_create(from);

    if (muc_active(jid->barejid)) {
        _private_chat_handler(stanza, jid->fulljid);
        jid_destroy(jid);
        return;
    }

    xmpp_ctx_t *ctx = connection_get_ctx();
    GDateTime *timestamp = stanza_get_delay(stanza);
    if (body) {
        char *message = xmpp_stanza_get_text(body);
        if (message) {
            char *enc_message = NULL;
            xmpp_stanza_t *x = xmpp_stanza_get_child_by_ns(stanza, STANZA_NS_ENCRYPTED);
            if (x) {
                enc_message = xmpp_stanza_get_text(x);
            }
            sv_ev_incoming_message(jid->barejid, jid->resourcepart, message, enc_message, timestamp);
            xmpp_free(ctx, enc_message);

            _receipt_request_handler(stanza);

            xmpp_free(ctx, message);
        }
    }

    if (!timestamp && jid->resourcepart) {
        gboolean gone = xmpp_stanza_get_child_by_name(stanza, STANZA_NAME_GONE) != NULL;
        gboolean typing = xmpp_stanza_get_child_by_name(stanza, STANZA_NAME_COMPOSING) != NULL;
        gboolean paused = xmpp_stanza_get_child_by_name(stanza, STANZA_NAME_PAUSED) != NULL;
        gboolean inactive = xmpp_stanza_get_child_by_name(stanza, STANZA_NAME_INACTIVE) != NULL;
        if (gone) {
            sv_ev_gone(jid->barejid, jid->resourcepart);
        } else if (typing) {
            sv_ev_typing(jid->barejid, jid->resourcepart);
        } else if (paused) {
            sv_ev_paused(jid->barejid, jid->resourcepart);
        } else if (inactive) {
            sv_ev_inactive(jid->barejid, jid->resourcepart);
        } else if (stanza_contains_chat_state(stanza)) {
            sv_ev_activity(jid->barejid, jid->resourcepart, TRUE);
        } else {
            sv_ev_activity(jid->barejid, jid->resourcepart, FALSE);
        }
    }

    if (timestamp) g_date_time_unref(timestamp);
    jid_destroy(jid);
}
