static void iscsi_timed_check_events(void *opaque)
{
    IscsiLun *iscsilun = opaque;

     
    iscsi_service(iscsilun->iscsi, 0);

    if (iscsilun->request_timed_out) {
        iscsilun->request_timed_out = false;
        iscsi_reconnect(iscsilun->iscsi);
    }

     
    iscsi_set_events(iscsilun);

    timer_mod(iscsilun->event_timer,
              qemu_clock_get_ms(QEMU_CLOCK_REALTIME) + EVENT_INTERVAL);
}
