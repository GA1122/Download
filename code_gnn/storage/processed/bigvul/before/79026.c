UpdateDerivedKeymapFields(struct xkb_keymap *keymap)
{
    struct xkb_key *key;
    struct xkb_mod *mod;
    struct xkb_led *led;
    unsigned int i, j;

     
    xkb_keys_foreach(key, keymap)
        if (!ApplyInterpsToKey(keymap, key))
            return false;

     
    xkb_keys_foreach(key, keymap)
        xkb_mods_enumerate(i, mod, &keymap->mods)
            if (key->vmodmap & (1u << i))
                mod->mapping |= key->modmap;

     
    for (i = 0; i < keymap->num_types; i++) {
        ComputeEffectiveMask(keymap, &keymap->types[i].mods);

        for (j = 0; j < keymap->types[i].num_entries; j++) {
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].mods);
            ComputeEffectiveMask(keymap, &keymap->types[i].entries[j].preserve);
        }
    }

     
    xkb_keys_foreach(key, keymap)
        for (i = 0; i < key->num_groups; i++)
            for (j = 0; j < XkbKeyNumLevels(key, i); j++)
                UpdateActionMods(keymap, &key->groups[i].levels[j].action,
                                 key->modmap);

     
    xkb_leds_foreach(led, keymap)
        ComputeEffectiveMask(keymap, &led->mods);

     
    xkb_keys_foreach(key, keymap)
        keymap->num_groups = MAX(keymap->num_groups, key->num_groups);

    return true;
}
