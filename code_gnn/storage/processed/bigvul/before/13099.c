uint32_t qemu_fdt_alloc_phandle(void *fdt)
{
    static int phandle = 0x0;

     
    if (!phandle) {
        phandle = machine_phandle_start(current_machine);
    }

    if (!phandle) {
         
        phandle = 0x8000;
    }

    return phandle++;
}
