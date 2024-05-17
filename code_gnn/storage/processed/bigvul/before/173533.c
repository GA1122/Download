int offload_effects_bundle_hal_start_output(audio_io_handle_t output, int pcm_id)
{
 int ret = 0;
 struct listnode *node;
 char mixer_string[128];
 output_context_t * out_ctxt = NULL;

    ALOGV("%s output %d pcm_id %d", __func__, output, pcm_id);

 if (lib_init() != 0)
 return init_status;

    pthread_mutex_lock(&lock);
 if (get_output(output) != NULL) {
        ALOGW("%s output already started", __func__);
        ret = -ENOSYS;
 goto exit;
 }

    out_ctxt = (output_context_t *)
                                 malloc(sizeof(output_context_t));
    out_ctxt->handle = output;
    out_ctxt->pcm_device_id = pcm_id;

  
    snprintf(mixer_string, sizeof(mixer_string),
 "%s %d", "Audio Effects Config", out_ctxt->pcm_device_id);
    out_ctxt->mixer = mixer_open(MIXER_CARD);
 if (!out_ctxt->mixer) {
        ALOGE("Failed to open mixer");
        out_ctxt->ctl = NULL;
        ret = -EINVAL;
        free(out_ctxt);
 goto exit;
 } else {
        out_ctxt->ctl = mixer_get_ctl_by_name(out_ctxt->mixer, mixer_string);
 if (!out_ctxt->ctl) {
            ALOGE("mixer_get_ctl_by_name failed");
            mixer_close(out_ctxt->mixer);
            out_ctxt->mixer = NULL;
            ret = -EINVAL;
            free(out_ctxt);
 goto exit;
 }
 }

    list_init(&out_ctxt->effects_list);

    list_for_each(node, &created_effects_list) {
 effect_context_t *fx_ctxt = node_to_item(node,
 effect_context_t,
                                                 effects_list_node);
 if (fx_ctxt->out_handle == output) {
 if (fx_ctxt->ops.start)
                fx_ctxt->ops.start(fx_ctxt, out_ctxt);
            list_add_tail(&out_ctxt->effects_list, &fx_ctxt->output_node);
 }
 }
    list_add_tail(&active_outputs_list, &out_ctxt->outputs_list_node);
exit:
    pthread_mutex_unlock(&lock);
 return ret;
}
