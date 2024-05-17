static int select_devices(struct audio_device *adev,
 audio_usecase_t uc_id)
{
 snd_device_t out_snd_device = SND_DEVICE_NONE;
 snd_device_t in_snd_device = SND_DEVICE_NONE;
 struct audio_usecase *usecase = NULL;
 struct audio_usecase *vc_usecase = NULL;
 struct listnode *node;
 struct stream_in *active_input = NULL;
 struct stream_out *active_out;
 struct mixer_card *mixer_card;

    ALOGV("%s: usecase(%d)", __func__, uc_id);

 if (uc_id == USECASE_AUDIO_CAPTURE_HOTWORD)
 return 0;

    usecase = get_usecase_from_type(adev, PCM_CAPTURE|VOICE_CALL);
 if (usecase != NULL) {
        active_input = (struct stream_in *)usecase->stream;
 }

    usecase = get_usecase_from_id(adev, uc_id);
 if (usecase == NULL) {
        ALOGE("%s: Could not find the usecase(%d)", __func__, uc_id);
 return -EINVAL;
 }
    active_out = (struct stream_out *)usecase->stream;

 if (usecase->type == VOICE_CALL) {
        out_snd_device = get_output_snd_device(adev, active_out->devices);
        in_snd_device = get_input_snd_device(adev, active_out->devices);
        usecase->devices = active_out->devices;
 } else {
  
 if (adev->in_call) {
            vc_usecase = get_usecase_from_id(adev, USECASE_VOICE_CALL);
 if (usecase == NULL) {
                ALOGE("%s: Could not find the voice call usecase", __func__);
 } else {
                in_snd_device = vc_usecase->in_snd_device;
                out_snd_device = vc_usecase->out_snd_device;
 }
 }
 if (usecase->type == PCM_PLAYBACK) {
            usecase->devices = active_out->devices;
            in_snd_device = SND_DEVICE_NONE;
 if (out_snd_device == SND_DEVICE_NONE) {
                out_snd_device = get_output_snd_device(adev, active_out->devices);
 if (active_out == adev->primary_output &&
                        active_input &&
                        active_input->source == AUDIO_SOURCE_VOICE_COMMUNICATION) {
                    select_devices(adev, active_input->usecase);
 }
 }
 } else if (usecase->type == PCM_CAPTURE) {
            usecase->devices = ((struct stream_in *)usecase->stream)->devices;
            out_snd_device = SND_DEVICE_NONE;
 if (in_snd_device == SND_DEVICE_NONE) {
 if (active_input->source == AUDIO_SOURCE_VOICE_COMMUNICATION &&
                        adev->primary_output && !adev->primary_output->standby) {
                    in_snd_device = get_input_snd_device(adev, adev->primary_output->devices);
 } else {
                    in_snd_device = get_input_snd_device(adev, AUDIO_DEVICE_NONE);
 }
 }
 }
 }

 if (out_snd_device == usecase->out_snd_device &&
        in_snd_device == usecase->in_snd_device) {
 return 0;
 }

    ALOGV("%s: out_snd_device(%d: %s) in_snd_device(%d: %s)", __func__,
          out_snd_device, get_snd_device_display_name(out_snd_device),
          in_snd_device,  get_snd_device_display_name(in_snd_device));


  
 if (usecase->out_snd_device != SND_DEVICE_NONE) {
        disable_snd_device(adev, usecase, usecase->out_snd_device, false);
 }

 if (usecase->in_snd_device != SND_DEVICE_NONE) {
        disable_snd_device(adev, usecase, usecase->in_snd_device, false);
 }

  
 if (out_snd_device != SND_DEVICE_NONE) {
        enable_snd_device(adev, usecase, out_snd_device, false);
 }

 if (in_snd_device != SND_DEVICE_NONE) {
        enable_snd_device(adev, usecase, in_snd_device, false);
 }

    list_for_each(node, &usecase->mixer_list) {
         mixer_card = node_to_item(node, struct mixer_card, uc_list_node[usecase->id]);
         audio_route_update_mixer(mixer_card->audio_route);
 }

    usecase->in_snd_device = in_snd_device;
    usecase->out_snd_device = out_snd_device;

 return 0;
}
