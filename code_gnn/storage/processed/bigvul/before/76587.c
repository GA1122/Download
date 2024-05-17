static int send_pong_message(nw_ses *ses)
{
    return send_reply(ses, 0xa, NULL, 0);
}
