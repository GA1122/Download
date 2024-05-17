void WaitForGuestSurfaceReady(content::WebContents* guest_web_contents) {
  RenderWidgetHostViewChildFrame* child_view =
      static_cast<RenderWidgetHostViewChildFrame*>(
          guest_web_contents->GetRenderWidgetHostView());

  RenderWidgetHostViewBase* root_view = static_cast<RenderWidgetHostViewBase*>(
      static_cast<content::WebContentsImpl*>(guest_web_contents)
          ->GetOuterWebContents()
          ->GetRenderWidgetHostView());

  SurfaceHitTestReadyNotifier notifier(child_view);
  notifier.WaitForSurfaceReady(root_view);
}
