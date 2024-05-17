bool Layer::AddAnimation(scoped_ptr <Animation> animation) {
  if (!layer_animation_controller_->animation_registrar())
    return false;

  UMA_HISTOGRAM_BOOLEAN("Renderer.AnimationAddedToOrphanLayer",
                        !layer_tree_host_);
  layer_animation_controller_->AddAnimation(animation.Pass());
  SetNeedsCommit();
  return true;
}
