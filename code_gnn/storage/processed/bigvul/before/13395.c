static void i6300esb_reset(DeviceState *dev)
{
    PCIDevice *pdev = PCI_DEVICE(dev);
    I6300State *d = WATCHDOG_I6300ESB_DEVICE(pdev);

    i6300esb_debug("I6300State = %p\n", d);

    i6300esb_disable_timer(d);

     

    d->reboot_enabled = 1;
    d->clock_scale = CLOCK_SCALE_1KHZ;
    d->int_type = INT_TYPE_IRQ;
    d->free_run = 0;
    d->locked = 0;
    d->enabled = 0;
    d->timer1_preload = 0xfffff;
    d->timer2_preload = 0xfffff;
    d->stage = 1;
    d->unlock_state = 0;
}
