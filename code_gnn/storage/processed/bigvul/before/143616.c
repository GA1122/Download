void RenderWidgetHostImpl::FlushInput() {
  input_router_->RequestNotificationWhenFlushed();
  if (synthetic_gesture_controller_)
    synthetic_gesture_controller_->Flush(base::TimeTicks::Now());
}
