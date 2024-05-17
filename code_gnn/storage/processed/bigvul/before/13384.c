static uint32_t i6300esb_config_read(PCIDevice *dev, uint32_t addr, int len)
{
    I6300State *d = WATCHDOG_I6300ESB_DEVICE(dev);
    uint32_t data;

    i6300esb_debug ("addr = %x, len = %d\n", addr, len);

    if (addr == ESB_CONFIG_REG && len == 2) {
        data =
            (d->reboot_enabled ? 0 : ESB_WDT_REBOOT) |
            (d->clock_scale == CLOCK_SCALE_1MHZ ? ESB_WDT_FREQ : 0) |
            d->int_type;
        return data;
    } else if (addr == ESB_LOCK_REG && len == 1) {
        data =
            (d->free_run ? ESB_WDT_FUNC : 0) |
            (d->locked ? ESB_WDT_LOCK : 0) |
            (d->enabled ? ESB_WDT_ENABLE : 0);
        return data;
    } else {
        return pci_default_read_config(dev, addr, len);
    }
}
