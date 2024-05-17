parse_OUTPUT_TRUNC(const char *arg, struct ofpbuf *ofpacts OVS_UNUSED,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
     
    return xasprintf("unknown action %s", arg);
}
