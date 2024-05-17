static void i6300esb_config_write(PCIDevice *dev, uint32_t addr,
                                  uint32_t data, int len)
{
    I6300State *d = WATCHDOG_I6300ESB_DEVICE(dev);
    int old;

    i6300esb_debug("addr = %x, data = %x, len = %d\n", addr, data, len);

    if (addr == ESB_CONFIG_REG && len == 2) {
        d->reboot_enabled = (data & ESB_WDT_REBOOT) == 0;
        d->clock_scale =
            (data & ESB_WDT_FREQ) != 0 ? CLOCK_SCALE_1MHZ : CLOCK_SCALE_1KHZ;
        d->int_type = (data & ESB_WDT_INTTYPE);
    } else if (addr == ESB_LOCK_REG && len == 1) {
        if (!d->locked) {
            d->locked = (data & ESB_WDT_LOCK) != 0;
            d->free_run = (data & ESB_WDT_FUNC) != 0;
            old = d->enabled;
            d->enabled = (data & ESB_WDT_ENABLE) != 0;
            if (!old && d->enabled)  
                i6300esb_restart_timer(d, 1);
            else if (!d->enabled)
                i6300esb_disable_timer(d);
        }
    } else {
        pci_default_write_config(dev, addr, data, len);
    }
}
