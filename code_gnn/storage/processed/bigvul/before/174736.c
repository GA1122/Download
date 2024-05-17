static int vol_prc_lib_create(const effect_uuid_t *uuid,
 int32_t session_id,
 int32_t io_id,
 effect_handle_t *p_handle)
{
 int itt = 0;
 vol_listener_context_t *context = NULL;

    ALOGV("volume_prc_lib_create .. called ..");

 if (lib_init() != 0) {
 return init_status;
 }

 if (p_handle == NULL || uuid == NULL) {
        ALOGE("%s: %s is NULL", __func__, (p_handle == NULL) ? "p_handle" : "uuid");
 return -EINVAL;
 }

    context = (vol_listener_context_t *)calloc(1, sizeof(vol_listener_context_t));

 if (context == NULL) {
        ALOGE("%s: failed to allocate for context .. oops !!", __func__);
 return -EINVAL;
 }

 for (itt = 0; descriptors[itt] != NULL; itt++) {
 if (memcmp(uuid, &descriptors[itt]->uuid, sizeof(effect_uuid_t)) == 0) {
            context->desc = descriptors[itt];
            context->stream_type = itt;
            PRINT_STREAM_TYPE(itt)
 break;
 }
 }

 if (descriptors[itt] == NULL) {
        ALOGE("%s .. couldnt find passed uuid, something wrong", __func__);
        free(context);
 return -EINVAL;
 }

    ALOGV("%s CREATED_CONTEXT %p", __func__, context);

    context->itfe = &effect_interface;
    context->state = VOL_LISTENER_STATE_INITIALIZED;
    context->dev_id = AUDIO_DEVICE_NONE;
    context->session_id = session_id;

    pthread_mutex_lock(&vol_listner_init_lock);
    list_add_tail(&vol_effect_list, &context->effect_list_node);

 if (dumping_enabled) {
        dump_list_l();
 }

    pthread_mutex_unlock(&vol_listner_init_lock);

 *p_handle = (effect_handle_t)context;
 return 0;
}
