static void scoop_sysbus_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SysBusDeviceClass *k = SYS_BUS_DEVICE_CLASS(klass);

    k->init = scoop_init;
    dc->desc = "Scoop2 Sharp custom ASIC";
    dc->vmsd = &vmstate_scoop_regs;
    dc->props = scoop_sysbus_properties;
}
