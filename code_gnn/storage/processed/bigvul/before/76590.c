int ws_send_text(nw_ses *ses, char *message)
{
    return send_reply(ses, 0x1, message, strlen(message));
}
