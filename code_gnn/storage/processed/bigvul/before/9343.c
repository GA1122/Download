static void sysbus_esp_realize(DeviceState *dev, Error **errp)
{
    SysBusDevice *sbd = SYS_BUS_DEVICE(dev);
    SysBusESPState *sysbus = ESP(dev);
    ESPState *s = &sysbus->esp;
    Error *err = NULL;

    sysbus_init_irq(sbd, &s->irq);
    assert(sysbus->it_shift != -1);

    s->chip_id = TCHI_FAS100A;
    memory_region_init_io(&sysbus->iomem, OBJECT(sysbus), &sysbus_esp_mem_ops,
                          sysbus, "esp", ESP_REGS << sysbus->it_shift);
    sysbus_init_mmio(sbd, &sysbus->iomem);

    qdev_init_gpio_in(dev, sysbus_esp_gpio_demux, 2);

    scsi_bus_new(&s->bus, sizeof(s->bus), dev, &esp_scsi_info, NULL);
    scsi_bus_legacy_handle_cmdline(&s->bus, &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
}
