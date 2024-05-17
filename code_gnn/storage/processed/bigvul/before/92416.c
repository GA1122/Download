bool underline_enabled(void) {

        if (cached_underline_enabled < 0) {

                 

                if (colors_enabled())
                        cached_underline_enabled = !streq_ptr(getenv("TERM"), "linux");
                else
                        cached_underline_enabled = false;
        }

        return cached_underline_enabled;
}
