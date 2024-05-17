bool colors_enabled(void) {

         

        if (cached_colors_enabled < 0) {
                int val;

                val = getenv_bool("SYSTEMD_COLORS");
                if (val >= 0)
                        cached_colors_enabled = val;
                else if (getpid_cached() == 1)
                         
                        cached_colors_enabled = !getenv_terminal_is_dumb();
                else
                        cached_colors_enabled = !terminal_is_dumb();
        }

        return cached_colors_enabled;
}
