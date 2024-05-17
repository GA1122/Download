static int reds_security_check(RedLinkInfo *link)
{
    ChannelSecurityOptions *security_option = find_channel_security(link->link_mess->channel_type);
    uint32_t security = security_option ? security_option->options : default_channel_security;
    return (link->stream->ssl && (security & SPICE_CHANNEL_SECURITY_SSL)) ||
        (!link->stream->ssl && (security & SPICE_CHANNEL_SECURITY_NONE));
}
