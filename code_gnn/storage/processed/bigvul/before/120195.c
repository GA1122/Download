bool Layer::TransformIsAnimating() const {
  return layer_animation_controller_->IsAnimatingProperty(Animation::Transform);
}
