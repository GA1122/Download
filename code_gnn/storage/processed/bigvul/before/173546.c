static int init() {
 void *lib_handle;
 const effect_descriptor_t *desc;

 if (init_status <= 0)
 return init_status;

 if (access(EFFECTS_DESCRIPTOR_LIBRARY_PATH, R_OK) == 0) {
        lib_handle = dlopen(EFFECTS_DESCRIPTOR_LIBRARY_PATH, RTLD_NOW);
 if (lib_handle == NULL) {
            ALOGE("%s: DLOPEN failed for %s", __func__, EFFECTS_DESCRIPTOR_LIBRARY_PATH);
 } else {
            ALOGV("%s: DLOPEN successful for %s", __func__, EFFECTS_DESCRIPTOR_LIBRARY_PATH);
            desc = (const effect_descriptor_t *)dlsym(lib_handle,
 "qcom_product_aec_descriptor");
 if (desc)
                descriptors[AEC_ID] = desc;

            desc = (const effect_descriptor_t *)dlsym(lib_handle,
 "qcom_product_ns_descriptor");
 if (desc)
                descriptors[NS_ID] = desc;

 }
 }

    uuid_to_id_table[AEC_ID] = FX_IID_AEC;
    uuid_to_id_table[NS_ID] = FX_IID_NS;

    list_init(&session_list);

    init_status = 0;
 return init_status;
}
