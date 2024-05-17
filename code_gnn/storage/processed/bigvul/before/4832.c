FixKeyState(DeviceEvent *event, DeviceIntPtr keybd)
{
    int key = event->detail.key;

    if (event->type == ET_KeyPress) {
        DebugF("FixKeyState: Key %d %s\n", key,
               ((event->type == ET_KeyPress) ? "down" : "up"));
    }

    if (event->type == ET_KeyPress)
        set_key_down(keybd, key, KEY_PROCESSED);
    else if (event->type == ET_KeyRelease)
        set_key_up(keybd, key, KEY_PROCESSED);
    else
        FatalError("Impossible keyboard event");
}
