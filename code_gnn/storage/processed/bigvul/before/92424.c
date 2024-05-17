int main(int argc, char **argv) {
        _cleanup_free_ char
                *vc = NULL,
                *vc_keymap = NULL, *vc_keymap_toggle = NULL,
                *vc_font = NULL, *vc_font_map = NULL, *vc_font_unimap = NULL;
        _cleanup_close_ int fd = -1;
        bool utf8, keyboard_ok;
        unsigned idx = 0;
        int r;

        log_setup_service();

        umask(0022);

        if (argv[1])
                fd = verify_source_vc(&vc, argv[1]);
        else
                fd = find_source_vc(&vc, &idx);
        if (fd < 0)
                return EXIT_FAILURE;

        utf8 = is_locale_utf8();

        r = parse_env_file(NULL, "/etc/vconsole.conf",
                           "KEYMAP", &vc_keymap,
                           "KEYMAP_TOGGLE", &vc_keymap_toggle,
                           "FONT", &vc_font,
                           "FONT_MAP", &vc_font_map,
                           "FONT_UNIMAP", &vc_font_unimap);
        if (r < 0 && r != -ENOENT)
                log_warning_errno(r, "Failed to read /etc/vconsole.conf: %m");

         
        r = proc_cmdline_get_key_many(
                        PROC_CMDLINE_STRIP_RD_PREFIX,
                        "vconsole.keymap", &vc_keymap,
                        "vconsole.keymap_toggle", &vc_keymap_toggle,
                        "vconsole.font", &vc_font,
                        "vconsole.font_map", &vc_font_map,
                        "vconsole.font_unimap", &vc_font_unimap,
                         
                        "vconsole.keymap.toggle", &vc_keymap_toggle,
                        "vconsole.font.map", &vc_font_map,
                        "vconsole.font.unimap", &vc_font_unimap);
        if (r < 0 && r != -ENOENT)
                log_warning_errno(r, "Failed to read /proc/cmdline: %m");

        (void) toggle_utf8_sysfs(utf8);
        (void) toggle_utf8(vc, fd, utf8);

        r = font_load_and_wait(vc, vc_font, vc_font_map, vc_font_unimap);
        keyboard_ok = keyboard_load_and_wait(vc, vc_keymap, vc_keymap_toggle, utf8) == 0;

        if (idx > 0) {
                if (r == 0)
                        setup_remaining_vcs(fd, idx, utf8);
                else if (r == EX_OSERR)
                         
                        log_notice("Setting fonts failed with a \"system error\", ignoring.");
                else
                        log_warning("Setting source virtual console failed, ignoring remaining ones");
        }

        return IN_SET(r, 0, EX_OSERR) && keyboard_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}