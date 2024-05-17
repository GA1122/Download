read_rssi_compensation_param(struct ar6_softc *ar)
{
    u8 *cust_data_ptr;


#ifdef RSSICOMPENSATION_PRINT
    s16 i;
    cust_data_ptr = ar6000_get_cust_data_buffer(ar->arTargetType);
    for (i=0; i<16; i++) {
        A_PRINTF("cust_data_%d = %x \n", i, *(u8 *)cust_data_ptr);
        cust_data_ptr += 1;
    }
#endif

    cust_data_ptr = ar6000_get_cust_data_buffer(ar->arTargetType);

    rssi_compensation_param.customerID = *(u16 *)cust_data_ptr & 0xffff;
    rssi_compensation_param.enable = *(u16 *)(cust_data_ptr+2) & 0xffff;
    rssi_compensation_param.bg_param_a = *(u16 *)(cust_data_ptr+4) & 0xffff;
    rssi_compensation_param.bg_param_b = *(u16 *)(cust_data_ptr+6) & 0xffff;
    rssi_compensation_param.a_param_a = *(u16 *)(cust_data_ptr+8) & 0xffff;
    rssi_compensation_param.a_param_b = *(u16 *)(cust_data_ptr+10) &0xffff;
    rssi_compensation_param.reserved = *(u32 *)(cust_data_ptr+12);

#ifdef RSSICOMPENSATION_PRINT
    A_PRINTF("customerID = 0x%x \n", rssi_compensation_param.customerID);
    A_PRINTF("enable = 0x%x \n", rssi_compensation_param.enable);
    A_PRINTF("bg_param_a = 0x%x and %d \n", rssi_compensation_param.bg_param_a, rssi_compensation_param.bg_param_a);
    A_PRINTF("bg_param_b = 0x%x and %d \n", rssi_compensation_param.bg_param_b, rssi_compensation_param.bg_param_b);
    A_PRINTF("a_param_a = 0x%x and %d \n", rssi_compensation_param.a_param_a, rssi_compensation_param.a_param_a);
    A_PRINTF("a_param_b = 0x%x and %d \n", rssi_compensation_param.a_param_b, rssi_compensation_param.a_param_b);
    A_PRINTF("Last 4 bytes = 0x%x \n", rssi_compensation_param.reserved);
#endif

    if (rssi_compensation_param.enable != 0x1) {
        rssi_compensation_param.enable = 0;
    }

   return;
}