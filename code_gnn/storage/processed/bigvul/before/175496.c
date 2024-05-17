static ssize_t read_bytes_from_dsp(struct stream_in *in, void* buffer,
 size_t bytes)
{
 struct pcm_device *pcm_device;
 struct audio_device *adev = in->dev;

    pcm_device = node_to_item(list_head(&in->pcm_dev_list),
 struct pcm_device, stream_list_node);

 if (pcm_device->sound_trigger_handle > 0)
 return adev->sound_trigger_read_samples(
                pcm_device->sound_trigger_handle, buffer, bytes);
 else
 return 0;
}
