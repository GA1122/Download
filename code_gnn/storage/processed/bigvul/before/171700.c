static void btif_context_switched(void *p_msg)
{

    BTIF_TRACE_VERBOSE("btif_context_switched");

    tBTIF_CONTEXT_SWITCH_CBACK *p = (tBTIF_CONTEXT_SWITCH_CBACK *) p_msg;

  
 if (p->p_cb)
        p->p_cb(p->event, p->p_param);
}
