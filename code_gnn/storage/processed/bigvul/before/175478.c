static int out_close_pcm_devices(struct stream_out *out)
{
 struct pcm_device *pcm_device;
 struct listnode *node;
 struct audio_device *adev = out->dev;

    list_for_each(node, &out->pcm_dev_list) {
        pcm_device = node_to_item(node, struct pcm_device, stream_list_node);
 if (pcm_device->sound_trigger_handle > 0) {
            adev->sound_trigger_close_for_streaming(
                    pcm_device->sound_trigger_handle);
            pcm_device->sound_trigger_handle = 0;
 }
 if (pcm_device->pcm) {
            pcm_close(pcm_device->pcm);
            pcm_device->pcm = NULL;
 }
 if (pcm_device->resampler) {
            release_resampler(pcm_device->resampler);
            pcm_device->resampler = NULL;
 }
 if (pcm_device->res_buffer) {
            free(pcm_device->res_buffer);
            pcm_device->res_buffer = NULL;
 }
 if (pcm_device->dsp_context) {
            cras_dsp_context_free(pcm_device->dsp_context);
            pcm_device->dsp_context = NULL;
 }
 }

 return 0;
}
