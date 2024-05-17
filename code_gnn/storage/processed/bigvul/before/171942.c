static void bta_pan_callback(tBTA_PAN_EVT event, tBTA_PAN *p_data)
{
    btif_transfer_context(bta_pan_callback_transfer, event, (char*)p_data, sizeof(tBTA_PAN), NULL);
}
