void esp_init(hwaddr espaddr, int it_shift,
              ESPDMAMemoryReadWriteFunc dma_memory_read,
              ESPDMAMemoryReadWriteFunc dma_memory_write,
              void *dma_opaque, qemu_irq irq, qemu_irq *reset,
              qemu_irq *dma_enable)
{
    DeviceState *dev;
    SysBusDevice *s;
    SysBusESPState *sysbus;
    ESPState *esp;

    dev = qdev_create(NULL, TYPE_ESP);
    sysbus = ESP(dev);
    esp = &sysbus->esp;
    esp->dma_memory_read = dma_memory_read;
    esp->dma_memory_write = dma_memory_write;
    esp->dma_opaque = dma_opaque;
    sysbus->it_shift = it_shift;
     
    esp->dma_enabled = 1;
    qdev_init_nofail(dev);
    s = SYS_BUS_DEVICE(dev);
    sysbus_connect_irq(s, 0, irq);
    sysbus_mmio_map(s, 0, espaddr);
    *reset = qdev_get_gpio_in(dev, 0);
    *dma_enable = qdev_get_gpio_in(dev, 1);
}