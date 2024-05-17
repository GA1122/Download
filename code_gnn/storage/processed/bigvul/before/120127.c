bool Layer::OpacityIsAnimating() const {
  return layer_animation_controller_->IsAnimatingProperty(Animation::Opacity);
}
