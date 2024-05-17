void bdt_disable(void)
{
    bdt_log("DISABLE BT");
 if (!bt_enabled) {
        bdt_log("Bluetooth is already disabled");
 return;
 }
    status = sBtInterface->disable();

    check_return_status(status);
}
