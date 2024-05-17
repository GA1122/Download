ar6000_sysfs_bmi_init(struct ar6_softc *ar)
{
    int status;

    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("BMI: Creating sysfs entry\n"));
    A_MEMZERO(&ar->osDevInfo, sizeof(struct hif_device_os_device_info));

     
    status = HIFConfigureDevice(ar->arHifDevice,
                                HIF_DEVICE_GET_OS_DEVICE,
                                &ar->osDevInfo,
                                sizeof(struct hif_device_os_device_info));

    if (status) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("BMI: Failed to get OS device info from HIF\n"));
        return A_ERROR;
    }

     
    if ((sysfs_create_bin_file(&(((struct device *)ar->osDevInfo.pOSDevice)->kobj), &bmi_attr)) < 0)
    {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("BMI: Failed to create entry for bmi in sysfs filesystem\n"));
        return A_ERROR;
    }

    return 0;
}