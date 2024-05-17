void msix_set_message(PCIDevice *dev, int vector, struct MSIMessage msg)
{
    uint8_t *table_entry = dev->msix_table + vector * PCI_MSIX_ENTRY_SIZE;

    pci_set_quad(table_entry + PCI_MSIX_ENTRY_LOWER_ADDR, msg.address);
    pci_set_long(table_entry + PCI_MSIX_ENTRY_DATA, msg.data);
    table_entry[PCI_MSIX_ENTRY_VECTOR_CTRL] &= ~PCI_MSIX_ENTRY_CTRL_MASKBIT;
}
