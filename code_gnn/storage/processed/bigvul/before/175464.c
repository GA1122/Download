static int in_release_pcm_devices(struct stream_in *in)
{
 struct pcm_device *pcm_device;
 struct listnode *node;
 struct listnode *next;

    list_for_each_safe(node, next, &in->pcm_dev_list) {
        pcm_device = node_to_item(node, struct pcm_device, stream_list_node);
        list_remove(node);
        free(pcm_device);
 }

 return 0;
}
