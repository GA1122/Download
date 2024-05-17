static void vapic_write(void *opaque, hwaddr addr, uint64_t data,
                        unsigned int size)
{
    VAPICROMState *s = opaque;
    X86CPU *cpu;
    CPUX86State *env;
    hwaddr rom_paddr;

    if (!current_cpu) {
        return;
    }

    cpu_synchronize_state(current_cpu);
    cpu = X86_CPU(current_cpu);
    env = &cpu->env;

     
    switch (size) {
    case 2:
        if (s->state == VAPIC_INACTIVE) {
            rom_paddr = (env->segs[R_CS].base + env->eip) & ROM_BLOCK_MASK;
            s->rom_state_paddr = rom_paddr + data;

            s->state = VAPIC_STANDBY;
        }
        if (vapic_prepare(s) < 0) {
            s->state = VAPIC_INACTIVE;
            s->rom_state_paddr = 0;
            break;
        }
        break;
    case 1:
        if (kvm_enabled()) {
             
            pause_all_vcpus();
            patch_byte(cpu, env->eip - 2, 0x66);
            patch_byte(cpu, env->eip - 1, 0x90);
            resume_all_vcpus();
        }

        if (s->state == VAPIC_ACTIVE) {
            break;
        }
        if (update_rom_mapping(s, env, env->eip) < 0) {
            break;
        }
        if (find_real_tpr_addr(s, env) < 0) {
            break;
        }
        vapic_enable(s, cpu);
        break;
    default:
    case 4:
        if (!kvm_irqchip_in_kernel()) {
            apic_poll_irq(cpu->apic_state);
        }
        break;
    }
}
