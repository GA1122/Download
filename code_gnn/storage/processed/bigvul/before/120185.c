void Layer::SetScrollClipLayerId(int clip_layer_id) {
  DCHECK(IsPropertyChangeAllowed());
  if (scroll_clip_layer_id_ == clip_layer_id)
    return;
  scroll_clip_layer_id_ = clip_layer_id;
  SetNeedsCommit();
}
