int offload_effects_bundle_hal_stop_output(audio_io_handle_t output, int pcm_id)
{
 int ret;
 struct listnode *node;
 struct listnode *fx_node;
 output_context_t *out_ctxt;

    ALOGV("%s output %d pcm_id %d", __func__, output, pcm_id);

 if (lib_init() != 0)
 return init_status;

    pthread_mutex_lock(&lock);

    out_ctxt = get_output(output);
 if (out_ctxt == NULL) {
        ALOGW("%s output not started", __func__);
        ret = -ENOSYS;
 goto exit;
 }

 if (out_ctxt->mixer)
        mixer_close(out_ctxt->mixer);

    list_for_each(fx_node, &out_ctxt->effects_list) {
 effect_context_t *fx_ctxt = node_to_item(fx_node,
 effect_context_t,
                                                 output_node);
 if (fx_ctxt->ops.stop)
            fx_ctxt->ops.stop(fx_ctxt, out_ctxt);
 }

    list_remove(&out_ctxt->outputs_list_node);

    free(out_ctxt);

exit:
    pthread_mutex_unlock(&lock);
 return ret;
}
