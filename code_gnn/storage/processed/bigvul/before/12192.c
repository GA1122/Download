void qemu_spice_display_refresh(SimpleSpiceDisplay *ssd)
{
    dprint(3, "%s/%d:\n", __func__, ssd->qxl.id);
    graphic_hw_update(ssd->dcl.con);

    qemu_mutex_lock(&ssd->lock);
    if (QTAILQ_EMPTY(&ssd->updates) && ssd->ds) {
        qemu_spice_create_update(ssd);
        ssd->notify++;
    }
    qemu_spice_cursor_refresh_unlocked(ssd);
    qemu_mutex_unlock(&ssd->lock);

    if (ssd->notify) {
        ssd->notify = 0;
        qemu_spice_wakeup(ssd);
        dprint(2, "%s/%d: notify\n", __func__, ssd->qxl.id);
    }
}
