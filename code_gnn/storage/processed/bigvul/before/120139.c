void Layer::RemoveLayerAnimationEventObserver(
    LayerAnimationEventObserver* animation_observer) {
  layer_animation_controller_->RemoveEventObserver(animation_observer);
}
