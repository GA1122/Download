bool Layer::FilterIsAnimating() const {
  return layer_animation_controller_->IsAnimatingProperty(Animation::Filter);
}
