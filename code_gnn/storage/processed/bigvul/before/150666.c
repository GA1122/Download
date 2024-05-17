void InputHandlerProxy::SynchronouslyAnimate(base::TimeTicks time) {
  DCHECK(synchronous_input_handler_);
  DCHECK(!allow_root_animate_);
  base::AutoReset<bool> reset(&allow_root_animate_, true);
  Animate(time);
}
