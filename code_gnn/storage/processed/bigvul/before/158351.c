bool RenderWidgetHostImpl::RequestRepaintForTesting() {
  if (!view_)
    return false;

  return view_->RequestRepaintForTesting();
}
