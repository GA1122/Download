static int uc_release_pcm_devices(struct audio_usecase *usecase)
{
 struct stream_out *out = (struct stream_out *)usecase->stream;
 struct pcm_device *pcm_device;
 struct listnode *node;
 struct listnode *next;

    list_for_each_safe(node, next, &out->pcm_dev_list) {
        pcm_device = node_to_item(node, struct pcm_device, stream_list_node);
        list_remove(node);
        free(pcm_device);
 }
    list_init(&usecase->mixer_list);

 return 0;
}
