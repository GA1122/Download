int effect_lib_create(const effect_uuid_t *uuid,
 int32_t sessionId,
 int32_t ioId,
 effect_handle_t *pHandle) {
 int ret;
 int i;

    ALOGV("%s: sessionId: %d, ioId: %d", __func__, sessionId, ioId);
 if (lib_init() != 0)
 return init_status;

 if (pHandle == NULL || uuid == NULL)
 return -EINVAL;

 for (i = 0; descriptors[i] != NULL; i++) {
 if (memcmp(uuid, &descriptors[i]->uuid, sizeof(effect_uuid_t)) == 0)
 break;
 }

 if (descriptors[i] == NULL)
 return -EINVAL;

 effect_context_t *context;
 if (memcmp(uuid, &equalizer_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
 equalizer_context_t *eq_ctxt = (equalizer_context_t *)
                                       calloc(1, sizeof(equalizer_context_t));
        context = (effect_context_t *)eq_ctxt;
        context->ops.init = equalizer_init;
        context->ops.reset = equalizer_reset;
        context->ops.set_parameter = equalizer_set_parameter;
        context->ops.get_parameter = equalizer_get_parameter;
        context->ops.set_device = equalizer_set_device;
        context->ops.enable = equalizer_enable;
        context->ops.disable = equalizer_disable;
        context->ops.start = equalizer_start;
        context->ops.stop = equalizer_stop;

        context->desc = &equalizer_descriptor;
        eq_ctxt->ctl = NULL;
 } else if (memcmp(uuid, &bassboost_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
 bassboost_context_t *bass_ctxt = (bassboost_context_t *)
                                         calloc(1, sizeof(bassboost_context_t));
        context = (effect_context_t *)bass_ctxt;
        context->ops.init = bassboost_init;
        context->ops.reset = bassboost_reset;
        context->ops.set_parameter = bassboost_set_parameter;
        context->ops.get_parameter = bassboost_get_parameter;
        context->ops.set_device = bassboost_set_device;
        context->ops.enable = bassboost_enable;
        context->ops.disable = bassboost_disable;
        context->ops.start = bassboost_start;
        context->ops.stop = bassboost_stop;

        context->desc = &bassboost_descriptor;
        bass_ctxt->ctl = NULL;
 } else if (memcmp(uuid, &virtualizer_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
 virtualizer_context_t *virt_ctxt = (virtualizer_context_t *)
                                           calloc(1, sizeof(virtualizer_context_t));
        context = (effect_context_t *)virt_ctxt;
        context->ops.init = virtualizer_init;
        context->ops.reset = virtualizer_reset;
        context->ops.set_parameter = virtualizer_set_parameter;
        context->ops.get_parameter = virtualizer_get_parameter;
        context->ops.set_device = virtualizer_set_device;
        context->ops.enable = virtualizer_enable;
        context->ops.disable = virtualizer_disable;
        context->ops.start = virtualizer_start;
        context->ops.stop = virtualizer_stop;

        context->desc = &virtualizer_descriptor;
        virt_ctxt->ctl = NULL;
 } else if ((memcmp(uuid, &aux_env_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) ||
 (memcmp(uuid, &ins_env_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) ||
 (memcmp(uuid, &aux_preset_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) ||
 (memcmp(uuid, &ins_preset_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0)) {
 reverb_context_t *reverb_ctxt = (reverb_context_t *)
                                        calloc(1, sizeof(reverb_context_t));
        context = (effect_context_t *)reverb_ctxt;
        context->ops.init = reverb_init;
        context->ops.reset = reverb_reset;
        context->ops.set_parameter = reverb_set_parameter;
        context->ops.get_parameter = reverb_get_parameter;
        context->ops.set_device = reverb_set_device;
        context->ops.enable = reverb_enable;
        context->ops.disable = reverb_disable;
        context->ops.start = reverb_start;
        context->ops.stop = reverb_stop;

 if (memcmp(uuid, &aux_env_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
            context->desc = &aux_env_reverb_descriptor;
            reverb_auxiliary_init(reverb_ctxt);
 } else if (memcmp(uuid, &ins_env_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
            context->desc = &ins_env_reverb_descriptor;
            reverb_insert_init(reverb_ctxt);
 } else if (memcmp(uuid, &aux_preset_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
            context->desc = &aux_preset_reverb_descriptor;
            reverb_auxiliary_init(reverb_ctxt);
 } else if (memcmp(uuid, &ins_preset_reverb_descriptor.uuid,
 sizeof(effect_uuid_t)) == 0) {
            context->desc = &ins_preset_reverb_descriptor;
            reverb_preset_init(reverb_ctxt);
 }
        reverb_ctxt->ctl = NULL;
 } else {
 return -EINVAL;
 }

    context->itfe = &effect_interface;
    context->state = EFFECT_STATE_UNINITIALIZED;
    context->out_handle = (audio_io_handle_t)ioId;

    ret = context->ops.init(context);
 if (ret < 0) {
        ALOGW("%s init failed", __func__);
        free(context);
 return ret;
 }

    context->state = EFFECT_STATE_INITIALIZED;

    pthread_mutex_lock(&lock);
    list_add_tail(&created_effects_list, &context->effects_list_node);
 output_context_t *out_ctxt = get_output(ioId);
 if (out_ctxt != NULL)
        add_effect_to_output(out_ctxt, context);
    pthread_mutex_unlock(&lock);

 *pHandle = (effect_handle_t)context;

    ALOGV("%s created context %p", __func__, context);

 return 0;

}
