bool dev_console_colors_enabled(void) {
        _cleanup_free_ char *s = NULL;
        int b;

         

        b = getenv_bool("SYSTEMD_COLORS");
        if (b >= 0)
                return b;

        if (getenv_for_pid(1, "TERM", &s) <= 0)
                (void) proc_cmdline_get_key("TERM", 0, &s);

        return !streq_ptr(s, "dumb");
}
