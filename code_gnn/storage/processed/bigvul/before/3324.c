static void fdctrl_handle_tc(void *opaque, int irq, int level)
{

    if (level) {
        FLOPPY_DPRINTF("TC pulsed\n");
    }
}
