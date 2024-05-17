void LayerTreeHostImpl::MouseMoveAt(const gfx::Point& viewport_point) {
  gfx::PointF device_viewport_point = gfx::ScalePoint(
      gfx::PointF(viewport_point), active_tree_->device_scale_factor());
  LayerImpl* layer_impl =
      active_tree_->FindLayerThatIsHitByPoint(device_viewport_point);

  int new_id = Layer::INVALID_ID;
  if (layer_impl && layer_impl->ToScrollbarLayer())
    new_id = layer_impl->ToScrollbarLayer()->ScrollLayerId();
  if (new_id == Layer::INVALID_ID) {
    bool scroll_on_main_thread = false;
    uint32_t main_thread_scrolling_reasons;
    LayerImpl* scroll_layer_impl = FindScrollLayerForDeviceViewportPoint(
        device_viewport_point, InputHandler::TOUCHSCREEN, layer_impl,
        &scroll_on_main_thread, &main_thread_scrolling_reasons);

    if (scroll_layer_impl == InnerViewportScrollLayer())
      scroll_layer_impl = OuterViewportScrollLayer();

    new_id = scroll_layer_impl ? scroll_layer_impl->id() : Layer::INVALID_ID;
  }

  if (new_id != scroll_layer_id_mouse_currently_over_) {
    ScrollbarAnimationController* old_animation_controller =
        ScrollbarAnimationControllerForId(
            scroll_layer_id_mouse_currently_over_);
    if (old_animation_controller) {
      old_animation_controller->DidMouseLeave();
    }
    scroll_layer_id_mouse_currently_over_ = new_id;
  }

  ScrollbarAnimationController* new_animation_controller =
      ScrollbarAnimationControllerForId(new_id);
  if (!new_animation_controller)
    return;

  for (ScrollbarLayerImplBase* scrollbar : ScrollbarsFor(new_id)) {
    new_animation_controller->DidMouseMoveNear(
        scrollbar->orientation(),
        DeviceSpaceDistanceToLayer(device_viewport_point, scrollbar) /
            active_tree_->device_scale_factor());
  }
}
