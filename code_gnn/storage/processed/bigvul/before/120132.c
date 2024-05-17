void Layer::RemoveAnimation(int animation_id) {
  layer_animation_controller_->RemoveAnimation(animation_id);
  SetNeedsCommit();
}
