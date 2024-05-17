void RenderWidgetHostImpl::ProgressFling(base::TimeTicks current_time) {
  if (input_router_)
    input_router_->ProgressFling(current_time);
}
