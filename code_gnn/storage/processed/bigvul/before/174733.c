static int vol_effect_command(effect_handle_t self,
 uint32_t cmd_code, uint32_t cmd_size,
 void *p_cmd_data, uint32_t *reply_size,
 void *p_reply_data)
{
 vol_listener_context_t *context = (vol_listener_context_t *)self;
 int status = 0;

    ALOGV("%s Called ", __func__);
    pthread_mutex_lock(&vol_listner_init_lock);

 if (context == NULL || context->state == VOL_LISTENER_STATE_UNINITIALIZED) {
        ALOGE("%s: %s is NULL", __func__, (context == NULL) ?
 "context" : "context->state");
        status = -EINVAL;
 goto exit;
 }

 switch (cmd_code) {
 case EFFECT_CMD_INIT:
        ALOGV("%s :: cmd called EFFECT_CMD_INIT", __func__);
 if (p_reply_data == NULL || *reply_size != sizeof(int)) {
            ALOGE("%s: EFFECT_CMD_INIT: %s, sending -EINVAL", __func__,
 (p_reply_data == NULL) ? "p_reply_data is NULL" :
 "*reply_size != sizeof(int)");
 return -EINVAL;
 }
 *(int *)p_reply_data = 0;
 break;

 case EFFECT_CMD_SET_CONFIG:
        ALOGV("%s :: cmd called EFFECT_CMD_SET_CONFIG", __func__);
 break;

 case EFFECT_CMD_GET_CONFIG:
        ALOGV("%s :: cmd called EFFECT_CMD_GET_CONFIG", __func__);
 break;

 case EFFECT_CMD_RESET:
        ALOGV("%s :: cmd called EFFECT_CMD_RESET", __func__);
 break;

 case EFFECT_CMD_SET_AUDIO_MODE:
        ALOGV("%s :: cmd called EFFECT_CMD_SET_AUDIO_MODE", __func__);
 break;

 case EFFECT_CMD_OFFLOAD:
        ALOGV("%s :: cmd called EFFECT_CMD_OFFLOAD", __func__);
 if (p_reply_data == NULL || *reply_size != sizeof(int)) {
            ALOGE("%s: EFFECT_CMD_OFFLOAD: %s, sending -EINVAL", __func__,
 (p_reply_data == NULL) ? "p_reply_data is NULL" :
 "*reply_size != sizeof(int)");
 return -EINVAL;
 }
 *(int *)p_reply_data = 0;
 break;

 case EFFECT_CMD_ENABLE:
        ALOGV("%s :: cmd called EFFECT_CMD_ENABLE", __func__);
 if (p_reply_data == NULL || *reply_size != sizeof(int)) {
            ALOGE("%s: EFFECT_CMD_ENABLE: %s, sending -EINVAL", __func__,
 (p_reply_data == NULL) ? "p_reply_data is NULL" :
 "*reply_size != sizeof(int)");
            status = -EINVAL;
 goto exit;
 }

 if (context->state != VOL_LISTENER_STATE_INITIALIZED) {
            ALOGE("%s: EFFECT_CMD_ENABLE : state not INITIALIZED", __func__);
            status = -ENOSYS;
 goto exit;
 }

        context->state = VOL_LISTENER_STATE_ACTIVE;
 *(int *)p_reply_data = 0;

 if (context->dev_id & AUDIO_DEVICE_OUT_SPEAKER) {
                check_and_set_gain_dep_cal();
 }

 break;

 case EFFECT_CMD_DISABLE:
        ALOGV("%s :: cmd called EFFECT_CMD_DISABLE", __func__);
 if (p_reply_data == NULL || *reply_size != sizeof(int)) {
            ALOGE("%s: EFFECT_CMD_DISABLE: %s, sending -EINVAL", __func__,
 (p_reply_data == NULL) ? "p_reply_data is NULL" :
 "*reply_size != sizeof(int)");
            status = -EINVAL;
 goto exit;
 }

 if (context->state != VOL_LISTENER_STATE_ACTIVE) {
            ALOGE("%s: EFFECT_CMD_ENABLE : state not ACTIVE", __func__);
            status = -ENOSYS;
 goto exit;
 }

        context->state = VOL_LISTENER_STATE_INITIALIZED;
 *(int *)p_reply_data = 0;

 if (context->dev_id & AUDIO_DEVICE_OUT_SPEAKER) {
            check_and_set_gain_dep_cal();
 }

 break;

 case EFFECT_CMD_GET_PARAM:
        ALOGV("%s :: cmd called EFFECT_CMD_GET_PARAM", __func__);
 break;

 case EFFECT_CMD_SET_PARAM:
        ALOGV("%s :: cmd called EFFECT_CMD_SET_PARAM", __func__);
 break;

 case EFFECT_CMD_SET_DEVICE:
 {
 uint32_t new_device;
 bool recompute_gain_dep_cal_Level = false;
        ALOGV("cmd called EFFECT_CMD_SET_DEVICE ");

 if (p_cmd_data == NULL) {
            ALOGE("%s: EFFECT_CMD_SET_DEVICE: cmd data NULL", __func__);
            status = -EINVAL;
 goto exit;
 }

        new_device = *(uint32_t *)p_cmd_data;
        ALOGV("%s :: EFFECT_CMD_SET_DEVICE: (current/new) device (0x%x / 0x%x)",
               __func__, context->dev_id, new_device);

 if ((context->dev_id & AUDIO_DEVICE_OUT_SPEAKER) ||
 (new_device & AUDIO_DEVICE_OUT_SPEAKER)) {
            recompute_gain_dep_cal_Level = true;
 }

        context->dev_id = new_device;

 if (recompute_gain_dep_cal_Level) {
            check_and_set_gain_dep_cal();
 }
 }
 break;

 case EFFECT_CMD_SET_VOLUME:
 {
 float left_vol = 0, right_vol = 0;
 bool recompute_gain_dep_cal_Level = false;

        ALOGV("cmd called EFFECT_CMD_SET_VOLUME");
 if (p_cmd_data == NULL || cmd_size != 2 * sizeof(uint32_t)) {
            ALOGE("%s: EFFECT_CMD_SET_VOLUME: %s", __func__, (p_cmd_data == NULL) ?
 "p_cmd_data is NULL" : "cmd_size issue");
            status = -EINVAL;
 goto exit;
 }

 if (context->dev_id & AUDIO_DEVICE_OUT_SPEAKER) {
            recompute_gain_dep_cal_Level = true;
 }

        left_vol = (float)(*(uint32_t *)p_cmd_data) / (1 << 24);
        right_vol = (float)(*((uint32_t *)p_cmd_data + 1)) / (1 << 24);
        ALOGV("Current Volume (%f / %f ) new Volume (%f / %f)", context->left_vol,
              context->right_vol, left_vol, right_vol);

        context->left_vol = left_vol;
        context->right_vol = right_vol;

 if (recompute_gain_dep_cal_Level) {
            check_and_set_gain_dep_cal();
 }
 }
 break;

 default:
        ALOGW("volume_listener_command invalid command %d", cmd_code);
        status = -ENOSYS;
 break;
 }

exit:
    pthread_mutex_unlock(&vol_listner_init_lock);
 return status;
}
