static void usb_ehci_pci_realize(PCIDevice *dev, Error **errp)
{
    EHCIPCIState *i = PCI_EHCI(dev);
    EHCIState *s = &i->ehci;
    uint8_t *pci_conf = dev->config;

    pci_set_byte(&pci_conf[PCI_CLASS_PROG], 0x20);

     
    pci_set_byte(&pci_conf[PCI_CAPABILITY_LIST], 0x00);
     

    pci_set_byte(&pci_conf[PCI_INTERRUPT_PIN], 4);  
    pci_set_byte(&pci_conf[PCI_MIN_GNT], 0);
    pci_set_byte(&pci_conf[PCI_MAX_LAT], 0);

      

    pci_set_byte(&pci_conf[USB_SBRN], USB_RELEASE_2);  
    pci_set_byte(&pci_conf[0x61], 0x20);   
    pci_set_word(&pci_conf[0x62], 0x00);   

    pci_conf[0x64] = 0x00;
    pci_conf[0x65] = 0x00;
    pci_conf[0x66] = 0x00;
    pci_conf[0x67] = 0x00;
    pci_conf[0x68] = 0x01;
    pci_conf[0x69] = 0x00;
    pci_conf[0x6a] = 0x00;
    pci_conf[0x6b] = 0x00;   
    pci_conf[0x6c] = 0x00;
    pci_conf[0x6d] = 0x00;
    pci_conf[0x6e] = 0x00;
    pci_conf[0x6f] = 0xc0;   

    s->irq = pci_allocate_irq(dev);
    s->as = pci_get_address_space(dev);

    usb_ehci_realize(s, DEVICE(dev), NULL);
    pci_register_bar(dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mem);
}
