void InputHandlerProxy::Animate(base::TimeTicks time) {
  DCHECK(!input_handler_->IsCurrentlyScrollingViewport() ||
         allow_root_animate_);

  if (scroll_elasticity_controller_)
    scroll_elasticity_controller_->Animate(time);

  snap_fling_controller_->Animate(time);
}
