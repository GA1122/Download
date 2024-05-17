struct mixer_card *adev_get_mixer_for_card(struct audio_device *adev, int card)
{
 struct mixer_card *mixer_card;
 struct listnode *node;

    list_for_each(node, &adev->mixer_list) {
        mixer_card = node_to_item(node, struct mixer_card, adev_list_node);
 if (mixer_card->card == card)
 return mixer_card;
 }
 return NULL;
}
