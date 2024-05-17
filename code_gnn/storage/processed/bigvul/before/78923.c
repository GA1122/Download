CopyLedMapDefsToKeymap(struct xkb_keymap *keymap, CompatInfo *info)
{
    for (xkb_led_index_t idx = 0; idx < info->num_leds; idx++) {
        LedInfo *ledi = &info->leds[idx];
        xkb_led_index_t i;
        struct xkb_led *led;

         
        xkb_leds_enumerate(i, led, keymap)
            if (led->name == ledi->led.name)
                break;

         
        if (i >= keymap->num_leds) {
            log_dbg(keymap->ctx,
                    "Indicator name \"%s\" was not declared in the keycodes section; "
                    "Adding new indicator\n",
                    xkb_atom_text(keymap->ctx, ledi->led.name));

            xkb_leds_enumerate(i, led, keymap)
                if (led->name == XKB_ATOM_NONE)
                    break;

            if (i >= keymap->num_leds) {
                 
                if (i >= XKB_MAX_LEDS) {
                    log_err(keymap->ctx,
                            "Too many indicators (maximum is %d); "
                            "Indicator name \"%s\" ignored\n",
                            XKB_MAX_LEDS,
                            xkb_atom_text(keymap->ctx, ledi->led.name));
                    continue;
                }

                 
                led = &keymap->leds[keymap->num_leds++];
            }
        }

        *led = ledi->led;
        if (led->groups != 0 && led->which_groups == 0)
            led->which_groups = XKB_STATE_LAYOUT_EFFECTIVE;
        if (led->mods.mods != 0 && led->which_mods == 0)
            led->which_mods = XKB_STATE_MODS_EFFECTIVE;
    }
}
