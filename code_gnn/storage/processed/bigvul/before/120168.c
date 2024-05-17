void Layer::SetLayerAnimationControllerForTest(
    scoped_refptr<LayerAnimationController> controller) {
  layer_animation_controller_->RemoveValueObserver(this);
  layer_animation_controller_ = controller;
  layer_animation_controller_->AddValueObserver(this);
  SetNeedsCommit();
}
