static int scoop_init(SysBusDevice *sbd)
{
    DeviceState *dev = DEVICE(sbd);
    ScoopInfo *s = SCOOP(dev);

    s->status = 0x02;
    qdev_init_gpio_out(dev, s->handler, 16);
    qdev_init_gpio_in(dev, scoop_gpio_set, 16);
    memory_region_init_io(&s->iomem, OBJECT(s), &scoop_ops, s, "scoop", 0x1000);

    sysbus_init_mmio(sbd, &s->iomem);

    return 0;
}
