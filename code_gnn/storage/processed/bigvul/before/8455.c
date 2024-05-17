pvscsi_schedule_completion_processing(PVSCSIState *s)
{
     
    if (!QTAILQ_EMPTY(&s->completion_queue)) {
        qemu_bh_schedule(s->completion_worker);
    }
}
