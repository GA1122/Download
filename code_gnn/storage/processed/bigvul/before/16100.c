int session_activate(Session *s) {
        int r;

        assert(s);

        if (s->vtnr < 0)
                return -ENOTSUP;

        if (!s->seat)
                return -ENOTSUP;

        if (s->seat->active == s)
                return 0;

        assert(seat_is_vtconsole(s->seat));

        r = chvt(s->vtnr);
        if (r < 0)
                return r;

        return seat_set_active(s->seat, s);
}
