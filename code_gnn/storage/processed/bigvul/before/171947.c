static void btif_pan_close_all_conns() {
 if (!stack_initialized)
 return;

 for (int i = 0; i < MAX_PAN_CONNS; ++i)
 {
 if (btpan_cb.conns[i].handle != -1)
            BTA_PanClose(btpan_cb.conns[i].handle);
 }
}
