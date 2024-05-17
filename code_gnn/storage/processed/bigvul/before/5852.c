static void usb_ehci_vm_state_change(void *opaque, int running, RunState state)
{
    EHCIState *ehci = opaque;

     
    if (state == RUN_STATE_RUNNING) {
        ehci_advance_async_state(ehci);
    }

     
    if (state == RUN_STATE_SAVE_VM) {
        ehci_advance_async_state(ehci);
        ehci_queues_rip_unseen(ehci, 1);
    }
}
