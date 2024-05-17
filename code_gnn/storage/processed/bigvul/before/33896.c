child_timeout_callback(gpointer p)
{
    mainloop_child_t *child = p;

    child->timerid = 0;
    if (child->timeout) {
        crm_crit("%s process (PID %d) will not die!", child->desc, (int)child->pid);
        return FALSE;
    }

    child->timeout = TRUE;
    crm_warn("%s process (PID %d) timed out", child->desc, (int)child->pid);

    if (kill(child->pid, SIGKILL) < 0) {
        if (errno == ESRCH) {
             
            return FALSE;
        }
        crm_perror(LOG_ERR, "kill(%d, KILL) failed", child->pid);
    }

    child->timerid = g_timeout_add(5000, child_timeout_callback, child);
    return FALSE;
}