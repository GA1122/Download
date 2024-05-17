static int vol_effect_process(effect_handle_t self,
 audio_buffer_t *in_buffer,
 audio_buffer_t *out_buffer)
{
 int status = 0;
    ALOGV("%s Called ", __func__);

 vol_listener_context_t *context = (vol_listener_context_t *)self;
    pthread_mutex_lock(&vol_listner_init_lock);

 if (context->state != VOL_LISTENER_STATE_ACTIVE) {
        ALOGE("%s: state is not active .. return error", __func__);
        status = -EINVAL;
 goto exit;
 }

 if (in_buffer->raw != out_buffer->raw) {
        memcpy(out_buffer->raw, in_buffer->raw, out_buffer->frameCount * 2 * sizeof(int16_t));
 } else {
        ALOGW("%s: something wrong, didn't handle in_buffer and out_buffer same address case",
              __func__);
 }

exit:
    pthread_mutex_unlock(&vol_listner_init_lock);
 return status;
}
