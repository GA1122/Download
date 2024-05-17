crm_trigger_prepare(GSource * source, gint * timeout)
{
    crm_trigger_t *trig = (crm_trigger_t *) source;

     
    *timeout = 500;              

    return trig->trigger;
}
