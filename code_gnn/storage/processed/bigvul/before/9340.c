static void sysbus_esp_hard_reset(DeviceState *dev)
{
    SysBusESPState *sysbus = ESP(dev);
    esp_hard_reset(&sysbus->esp);
}
