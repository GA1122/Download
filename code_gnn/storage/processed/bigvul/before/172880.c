static void adapter_state_changed(bt_state_t state)
{
    bdt_log("ADAPTER STATE UPDATED : %s", (state == BT_STATE_OFF)?"OFF":"ON");
 if (state == BT_STATE_ON) {
        bt_enabled = 1;
 } else {
        bt_enabled = 0;
 }
}
