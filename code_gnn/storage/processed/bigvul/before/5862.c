void ahci_init(AHCIState *s, DeviceState *qdev)
{
    s->container = qdev;
     
    memory_region_init_io(&s->mem, OBJECT(qdev), &ahci_mem_ops, s,
                          "ahci", AHCI_MEM_BAR_SIZE);
    memory_region_init_io(&s->idp, OBJECT(qdev), &ahci_idp_ops, s,
                          "ahci-idp", 32);
}
