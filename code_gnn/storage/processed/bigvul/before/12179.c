static void interface_attach_worker(QXLInstance *sin, QXLWorker *qxl_worker)
{
    SimpleSpiceDisplay *ssd = container_of(sin, SimpleSpiceDisplay, qxl);

    dprint(1, "%s/%d:\n", __func__, ssd->qxl.id);
    ssd->worker = qxl_worker;
}
