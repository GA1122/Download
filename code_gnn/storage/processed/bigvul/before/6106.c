long ssl2_ctrl(SSL *s, int cmd, long larg, void *parg)
{
    int ret = 0;

    switch (cmd) {
    case SSL_CTRL_GET_SESSION_REUSED:
        ret = s->hit;
        break;
    case SSL_CTRL_CHECK_PROTO_VERSION:
        return ssl3_ctrl(s, SSL_CTRL_CHECK_PROTO_VERSION, larg, parg);
    default:
        break;
    }
    return (ret);
}
