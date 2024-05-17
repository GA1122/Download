clump_splay_walk_init_mid(clump_splay_walker *sw, clump_t *cp)
{
    sw->from = SPLAY_FROM_LEFT;
    sw->cp = cp;
    sw->end = cp;
    if (cp)
    {
        SANITY_CHECK_MID(cp);
    }
    return cp;
}
