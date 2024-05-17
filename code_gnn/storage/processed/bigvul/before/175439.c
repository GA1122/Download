void free_mixer_list(struct audio_device *adev)
{
 struct mixer_card *mixer_card;
 struct listnode *node;
 struct listnode *next;

    list_for_each_safe(node, next, &adev->mixer_list) {
        mixer_card = node_to_item(node, struct mixer_card, adev_list_node);
        list_remove(node);
        audio_route_free(mixer_card->audio_route);
        free(mixer_card);
 }
}
