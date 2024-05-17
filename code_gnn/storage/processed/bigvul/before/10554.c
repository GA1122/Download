static bool acpi_pcihp_pc_no_hotplug(AcpiPciHpState *s, PCIDevice *dev)
{
    PCIDeviceClass *pc = PCI_DEVICE_GET_CLASS(dev);
    DeviceClass *dc = DEVICE_GET_CLASS(dev);
     
    return (pc->is_bridge && !dev->qdev.hotplugged) || !dc->hotpluggable;
}
