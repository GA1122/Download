static void init_once()
{
 int i = 0;
 if (initialized) {
        ALOGV("%s : already init .. do nothing", __func__);
 return;
 }

    ALOGD("%s Called ", __func__);
    pthread_mutex_init(&vol_listner_init_lock, NULL);

 if (access(PRIMARY_HAL_PATH, R_OK) == 0) {
 void *hal_lib_pointer = dlopen(PRIMARY_HAL_PATH, RTLD_NOW);
 if (hal_lib_pointer == NULL) {
            ALOGE("%s: DLOPEN failed for %s", __func__, PRIMARY_HAL_PATH);
            send_gain_dep_cal = NULL;
 } else {
            ALOGV("%s: DLOPEN of %s Succes .. next get HAL entry function", __func__, PRIMARY_HAL_PATH);
            send_gain_dep_cal = (bool (*)(int))dlsym(hal_lib_pointer, AHAL_GAIN_DEPENDENT_INTERFACE_FUNCTION);
 if (send_gain_dep_cal == NULL) {
                ALOGE("Couldnt able to get the function symbol");
 }
 }
 } else {
        ALOGE("%s: not able to acces lib %s ", __func__, PRIMARY_HAL_PATH);
        send_gain_dep_cal = NULL;
 }

 char check_dump_val[PROPERTY_VALUE_MAX];
    property_get("audio.volume.listener.dump", check_dump_val, "0");
 if (atoi(check_dump_val)) {
        dumping_enabled = true;
 }

    init_status = 0;
    list_init(&vol_effect_list);
    initialized = true;
}
