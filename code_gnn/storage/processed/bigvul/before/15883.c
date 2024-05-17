static uint8_t *msix_pending_byte(PCIDevice *dev, int vector)
{
    return dev->msix_pba + vector / 8;
}
