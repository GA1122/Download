static void i6300esb_timer_expired(void *vp)
{
    I6300State *d = vp;

    i6300esb_debug("stage %d\n", d->stage);

    if (d->stage == 1) {
         
        switch (d->int_type) {
        case INT_TYPE_IRQ:
            fprintf(stderr, "i6300esb_timer_expired: I would send APIC 1 INT 10 here if I knew how (XXX)\n");
            break;
        case INT_TYPE_SMI:
            fprintf(stderr, "i6300esb_timer_expired: I would send SMI here if I knew how (XXX)\n");
            break;
        }

         
        i6300esb_restart_timer(d, 2);
    } else {
         
        if (d->reboot_enabled) {
            d->previous_reboot_flag = 1;
            watchdog_perform_action();  
            i6300esb_reset(&d->dev.qdev);
        }

         
        if (d->free_run)
            i6300esb_restart_timer(d, 1);
    }
}
