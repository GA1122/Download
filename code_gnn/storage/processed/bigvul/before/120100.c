void Layer::AddLayerAnimationEventObserver(
    LayerAnimationEventObserver* animation_observer) {
  layer_animation_controller_->AddEventObserver(animation_observer);
}
