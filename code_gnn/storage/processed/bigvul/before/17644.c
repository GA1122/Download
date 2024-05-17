CursorFreeClient(void *data, XID id)
{
    CursorEventPtr old = (CursorEventPtr) data;
    CursorEventPtr *prev, e;

    for (prev = &cursorEvents; (e = *prev); prev = &e->next) {
        if (e == old) {
            *prev = e->next;
            free(e);
            break;
        }
    }
    return 1;
}
