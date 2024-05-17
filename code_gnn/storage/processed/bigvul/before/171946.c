void btif_pan_cleanup()
{
 if (!stack_initialized)
 return;

 for (int i = 0; i < MAX_PAN_CONNS; i++)
        btpan_cleanup_conn(&btpan_cb.conns[i]);

    pan_disable();
    stack_initialized = false;
}
