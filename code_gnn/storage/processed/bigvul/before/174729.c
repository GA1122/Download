static void check_and_set_gain_dep_cal()
{

 struct listnode *node = NULL;
 float new_vol = 0.0;
 int max_level = 0;
 vol_listener_context_t *context = NULL;
 if (dumping_enabled) {
        dump_list_l();
 }

    ALOGV("%s ==> Start ...", __func__);

    list_for_each(node, &vol_effect_list) {
        context = node_to_item(node, struct vol_listener_context_s, effect_list_node);
 if ((context->state == VOL_LISTENER_STATE_ACTIVE) &&
 (context->dev_id & AUDIO_DEVICE_OUT_SPEAKER) &&
 (new_vol < (context->left_vol + context->right_vol) / 2)) {
            new_vol = (context->left_vol + context->right_vol) / 2;
 }
 }

 if (new_vol != current_vol) {
        ALOGV("%s:: Change in decision :: current volume is %f new volume is %f",
              __func__, current_vol, new_vol);

 if (send_gain_dep_cal != NULL) {
 int gain_dep_cal_level = -1;

 if (new_vol >= 1) {  
                gain_dep_cal_level = volume_curve_gain_mapping_table[MAX_GAIN_LEVELS - 1].level;
 } else if (new_vol <= 0) {
                gain_dep_cal_level = volume_curve_gain_mapping_table[0].level;
 } else {
 for (max_level = 0; max_level + 1 < MAX_GAIN_LEVELS; max_level++) {
 if (new_vol < volume_curve_gain_mapping_table[max_level + 1].amp &&
                        new_vol >= volume_curve_gain_mapping_table[max_level].amp) {
                        gain_dep_cal_level = volume_curve_gain_mapping_table[max_level].level;
                        ALOGV("%s: volume(%f), gain dep cal selcetd %d ",
                              __func__, current_vol, gain_dep_cal_level);
 break;
 }
 }
 }

 if (gain_dep_cal_level != -1) {
 if (gain_dep_cal_level != current_gain_dep_cal_level) {
 if (!send_gain_dep_cal(gain_dep_cal_level)) {
                        ALOGE("%s: Failed to set gain dep cal level", __func__);
 } else {
 if (dumping_enabled) {
                            ALOGW("%s: (old/new) Volume (%f/%f) (old/new) level (%d/%d)",
                                  __func__, current_vol, new_vol, current_gain_dep_cal_level,
                                  gain_dep_cal_level);
 } else {
                            ALOGV("%s: Change in Cal::(old/new) Volume (%f/%f) (old/new) level (%d/%d)",
                                  __func__, current_vol, new_vol, current_gain_dep_cal_level,
                                  gain_dep_cal_level);
 }
                        current_gain_dep_cal_level = gain_dep_cal_level;
                        current_vol = new_vol;
 }
 } else {
 if (dumping_enabled) {
                        ALOGW("%s: volume changed but gain dep cal level is still the same",
                              __func__);
 } else {
                        ALOGV("%s: volume changed but gain dep cal level is still the same",
                              __func__);
 }
 }
 } else {
                ALOGW("%s: Failed to find gain dep cal level for volume %f", __func__, new_vol);
 }
 } else {
            ALOGE("%s: not able to send calibration, NULL function pointer",
                  __func__);
 }
 } else {
        ALOGV("%s:: volume not changed, stick to same config ..... ", __func__);
 }

    ALOGV("check_and_set_gain_dep_cal ==> End ");
}
