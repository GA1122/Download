static void _vmxnet3_deassert_interrupt_line(VMXNET3State *s, int lidx)
{
    PCIDevice *d = PCI_DEVICE(s);

     
    assert(!s->msix_used || !msix_enabled(d));
     
    assert(!s->msi_used || !msi_enabled(d));

    VMW_IRPRN("Deasserting line for interrupt %u", lidx);
    pci_irq_deassert(d);
}
