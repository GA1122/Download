FindInterpForKey(struct xkb_keymap *keymap, const struct xkb_key *key,
                 xkb_layout_index_t group, xkb_level_index_t level)
{
    const xkb_keysym_t *syms;
    int num_syms;

    num_syms = xkb_keymap_key_get_syms_by_level(keymap, key->keycode, group,
                                                level, &syms);
    if (num_syms == 0)
        return NULL;

     
    for (unsigned i = 0; i < keymap->num_sym_interprets; i++) {
        const struct xkb_sym_interpret *interp = &keymap->sym_interprets[i];

        xkb_mod_mask_t mods;
        bool found = false;

        if ((num_syms > 1 || interp->sym != syms[0]) &&
            interp->sym != XKB_KEY_NoSymbol)
            continue;

        if (interp->level_one_only && level != 0)
            mods = 0;
        else
            mods = key->modmap;

        switch (interp->match) {
        case MATCH_NONE:
            found = !(interp->mods & mods);
            break;
        case MATCH_ANY_OR_NONE:
            found = (!mods || (interp->mods & mods));
            break;
        case MATCH_ANY:
            found = (interp->mods & mods);
            break;
        case MATCH_ALL:
            found = ((interp->mods & mods) == interp->mods);
            break;
        case MATCH_EXACTLY:
            found = (interp->mods == mods);
            break;
        }

        if (found)
            return interp;
    }

    return &default_interpret;
}
