wait_for_refresh(int offset, const char *prefix, int msec)
{
    int lpc = msec / 1000;
    struct timespec sleept = { 1, 0 };

    if (as_console == FALSE) {
        timer_id = g_timeout_add(msec, mon_timer_popped, NULL);
        return;
    }

    crm_notice("%sRefresh in %ds...", prefix ? prefix : "", lpc);
    while (lpc > 0) {
#if CURSES_ENABLED
        move(offset, 0);
 
        printw("%sRefresh in %ds...\n", prefix ? prefix : "", lpc);
        clrtoeol();
        refresh();
#endif
        lpc--;
        if (lpc == 0) {
            timer_id = g_timeout_add(1000, mon_timer_popped, NULL);
        } else {
            if (nanosleep(&sleept, NULL) != 0) {
                return;
            }
        }
    }
}