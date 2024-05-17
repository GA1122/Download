static bool vnc_has_job_locked(VncState *vs)
{
    VncJob *job;

    QTAILQ_FOREACH(job, &queue->jobs, next) {
        if (job->vs == vs || !vs) {
            return true;
        }
    }
    return false;
}
