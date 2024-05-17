vmxnet3_io_bar1_read(void *opaque, hwaddr addr, unsigned size)
{
        VMXNET3State *s = opaque;
        uint64_t ret = 0;

        switch (addr) {
         
        case VMXNET3_REG_VRRS:
            VMW_CBPRN("Read BAR1 [VMXNET3_REG_VRRS], size %d", size);
            ret = VMXNET3_DEVICE_REVISION;
            break;

         
        case VMXNET3_REG_UVRS:
            VMW_CBPRN("Read BAR1 [VMXNET3_REG_UVRS], size %d", size);
            ret = VMXNET3_DEVICE_VERSION;
            break;

         
        case VMXNET3_REG_CMD:
            VMW_CBPRN("Read BAR1 [VMXNET3_REG_CMD], size %d", size);
            ret = vmxnet3_get_command_status(s);
            break;

         
        case VMXNET3_REG_MACL:
            VMW_CBPRN("Read BAR1 [VMXNET3_REG_MACL], size %d", size);
            ret = vmxnet3_get_mac_low(&s->conf.macaddr);
            break;

         
        case VMXNET3_REG_MACH:
            VMW_CBPRN("Read BAR1 [VMXNET3_REG_MACH], size %d", size);
            ret = vmxnet3_get_mac_high(&s->conf.macaddr);
            break;

         
        case VMXNET3_REG_ICR:
            VMW_CBPRN("Read BAR1 [VMXNET3_REG_ICR], size %d", size);
            if (vmxnet3_interrupt_asserted(s, 0)) {
                vmxnet3_clear_interrupt(s, 0);
                ret = true;
            } else {
                ret = false;
            }
            break;

        default:
            VMW_CBPRN("Unknow read BAR1[%" PRIx64 "], %d bytes", addr, size);
            break;
        }

        return ret;
}