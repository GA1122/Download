static void ide_clear_hob(IDEBus *bus)
{
     
    bus->ifs[0].select &= ~(1 << 7);
    bus->ifs[1].select &= ~(1 << 7);
}