static int pl022_init(SysBusDevice *sbd)
{
    DeviceState *dev = DEVICE(sbd);
    PL022State *s = PL022(dev);

    memory_region_init_io(&s->iomem, OBJECT(s), &pl022_ops, s, "pl022", 0x1000);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq);
    s->ssi = ssi_create_bus(dev, "ssi");
    pl022_reset(s);
    vmstate_register(dev, -1, &vmstate_pl022, s);
    return 0;
}
