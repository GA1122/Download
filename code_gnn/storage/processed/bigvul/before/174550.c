tBTM_STATUS BTM_ReadLocalOobData(void)
{
    tBTM_STATUS status = BTM_SUCCESS;

 if (btsnd_hcic_read_local_oob_data() == FALSE)
        status = BTM_NO_RESOURCES;

 return status;
}
