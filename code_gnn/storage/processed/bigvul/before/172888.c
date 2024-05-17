void check_return_status(bt_status_t status)
{
 if (status != BT_STATUS_SUCCESS)
 {
        bdt_log("HAL REQUEST FAILED status : %d (%s)", status, dump_bt_status(status));
 }
 else
 {
        bdt_log("HAL REQUEST SUCCESS");
 }
}
