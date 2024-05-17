int config_hci_snoop_log(uint8_t enable)
{
    LOG_INFO("config_hci_snoop_log");

 if (!interface_ready())
 return BT_STATUS_NOT_READY;

    btsnoop_get_interface()->set_api_wants_to_log(enable);
 return BT_STATUS_SUCCESS;
}
