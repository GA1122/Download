bool vnc_has_job(VncState *vs)
{
    bool ret;

    vnc_lock_queue(queue);
    ret = vnc_has_job_locked(vs);
    vnc_unlock_queue(queue);
    return ret;
}
