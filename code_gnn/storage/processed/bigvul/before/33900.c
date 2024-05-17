crm_trigger_dispatch(GSource * source, GSourceFunc callback, gpointer userdata)
{
    int rc = TRUE;
    crm_trigger_t *trig = (crm_trigger_t *) source;

    if(trig->running) {
         
        return TRUE;
    }
    trig->trigger = FALSE;

    if (callback) {
        rc = callback(trig->user_data);
        if(rc < 0) {
            crm_trace("Trigger handler %p not yet complete", trig);
            trig->running = TRUE;
            rc = TRUE;
        }
    }
    return rc;
}
