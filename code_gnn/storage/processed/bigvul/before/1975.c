SPICE_GNUC_VISIBLE int spice_server_set_agent_mouse(SpiceServer *s, int enable)
{
    spice_assert(reds == s);
    agent_mouse = enable;
    reds_update_mouse_mode();
    return 0;
}
