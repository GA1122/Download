static void tsc210x_touchscreen_event(void *opaque,
                int x, int y, int z, int buttons_state)
{
    TSC210xState *s = opaque;
    int p = s->pressure;

    if (buttons_state) {
        s->x = x;
        s->y = y;
    }
    s->pressure = !!buttons_state;

     
    if (p != s->pressure)
        tsc210x_pin_update(s);
}
