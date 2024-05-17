void WebContentsImpl::OnFocusedElementChangedInFrame(
    RenderFrameHostImpl* frame,
    const gfx::Rect& bounds_in_root_view) {
  RenderWidgetHostViewBase* root_view =
      static_cast<RenderWidgetHostViewBase*>(GetRenderWidgetHostView());
  if (!root_view || !frame->GetView())
    return;

  gfx::Point origin = bounds_in_root_view.origin();
  origin += root_view->GetViewBounds().OffsetFromOrigin();
  gfx::Rect bounds_in_screen(origin, bounds_in_root_view.size());

  root_view->FocusedNodeChanged(frame->has_focused_editable_element(),
                                bounds_in_screen);

  FocusedNodeDetails details = {frame->has_focused_editable_element(),
                                bounds_in_screen};

  NotificationService::current()->Notify(
      NOTIFICATION_FOCUS_CHANGED_IN_PAGE,
      Source<RenderViewHost>(GetRenderViewHost()),
      Details<FocusedNodeDetails>(&details));
}
