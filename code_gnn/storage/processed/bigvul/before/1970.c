SPICE_GNUC_VISIBLE SpiceServer *spice_server_new(void)
{
     
    spice_assert(reds == NULL);

    reds = spice_new0(RedsState, 1);
    return reds;
}