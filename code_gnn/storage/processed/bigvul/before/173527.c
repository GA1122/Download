int effect_process(effect_handle_t self,
 audio_buffer_t *inBuffer __unused,
 audio_buffer_t *outBuffer __unused)
{
 effect_context_t * context = (effect_context_t *)self;
 int status = 0;

    ALOGW("%s Called ?????", __func__);

    pthread_mutex_lock(&lock);
 if (!effect_exists(context)) {
        status = -ENOSYS;
 goto exit;
 }

 if (context->state != EFFECT_STATE_ACTIVE) {
        status = -ENODATA;
 goto exit;
 }

exit:
    pthread_mutex_unlock(&lock);
 return status;
}
