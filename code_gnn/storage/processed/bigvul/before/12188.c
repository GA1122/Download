void qemu_spice_cursor_refresh_unlocked(SimpleSpiceDisplay *ssd)
{
    if (ssd->cursor) {
        assert(ssd->dcl.con);
        dpy_cursor_define(ssd->dcl.con, ssd->cursor);
        cursor_put(ssd->cursor);
        ssd->cursor = NULL;
    }
    if (ssd->mouse_x != -1 && ssd->mouse_y != -1) {
        assert(ssd->dcl.con);
        dpy_mouse_set(ssd->dcl.con, ssd->mouse_x, ssd->mouse_y, 1);
        ssd->mouse_x = -1;
        ssd->mouse_y = -1;
    }
}
