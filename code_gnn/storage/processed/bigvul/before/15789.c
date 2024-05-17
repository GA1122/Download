static void sysbus_ahci_reset(DeviceState *dev)
{
    SysbusAHCIState *s = SYSBUS_AHCI(dev);

    ahci_reset(&s->ahci);
}
