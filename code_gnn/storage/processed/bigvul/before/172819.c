static int read_energy_info()
{
 if (interface_ready() == FALSE)
 return BT_STATUS_NOT_READY;
    btif_dm_read_energy_info();
 return BT_STATUS_SUCCESS;
}
