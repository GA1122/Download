MSIMessage msix_get_message(PCIDevice *dev, unsigned vector)
{
    uint8_t *table_entry = dev->msix_table + vector * PCI_MSIX_ENTRY_SIZE;
    MSIMessage msg;

    msg.address = pci_get_quad(table_entry + PCI_MSIX_ENTRY_LOWER_ADDR);
    msg.data = pci_get_long(table_entry + PCI_MSIX_ENTRY_DATA);
    return msg;
}
