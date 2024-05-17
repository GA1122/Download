void bdt_le_test_mode(char *p)
{
 int cmd;
 unsigned char buf[3];
 int arg1, arg2, arg3;

    bdt_log("BT LE TEST MODE");
 if (!bt_enabled) {
        bdt_log("Bluetooth must be enabled for le_test to work.");
 return;
 }

    memset(buf, 0, sizeof(buf));
    cmd = get_int(&p, 0);
 switch (cmd)
 {
 case 0x1:  
           arg1 = get_int(&p, -1);
 if (arg1 < 0) bdt_log("%s Invalid arguments", __FUNCTION__);
           buf[0] = arg1;
           status = sBtInterface->le_test_mode(HCI_LE_RECEIVER_TEST_OPCODE, buf, 1);
 break;
 case 0x2:  
            arg1 = get_int(&p, -1);
            arg2 = get_int(&p, -1);
            arg3 = get_int(&p, -1);
 if ((arg1 < 0) || (arg2 < 0) || (arg3 < 0))
                bdt_log("%s Invalid arguments", __FUNCTION__);
            buf[0] = arg1;
            buf[1] = arg2;
            buf[2] = arg3;
            status = sBtInterface->le_test_mode(HCI_LE_TRANSMITTER_TEST_OPCODE, buf, 3);
 break;
 case 0x3:  
            status = sBtInterface->le_test_mode(HCI_LE_END_TEST_OPCODE, buf, 0);
 break;
 default:
            bdt_log("Unsupported command");
 return;
 break;
 }
 if (status != BT_STATUS_SUCCESS)
 {
        bdt_log("%s Test 0x%x Failed with status:0x%x", __FUNCTION__, cmd, status);
 }
 return;
}
