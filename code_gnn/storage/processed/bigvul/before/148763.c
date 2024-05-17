WebContentsView* InterstitialPageImpl::CreateWebContentsView() {
  if (!enabled() || !create_view_)
    return NULL;
  WebContentsView* wcv =
      static_cast<WebContentsImpl*>(web_contents())->GetView();
  RenderWidgetHostViewBase* view =
      wcv->CreateViewForWidget(render_view_host_->GetWidget(), false);
  RenderWidgetHostImpl::From(render_view_host_->GetWidget())->SetView(view);
  render_view_host_->GetMainFrame()->AllowBindings(
      BINDINGS_POLICY_DOM_AUTOMATION);

  render_view_host_->CreateRenderView(MSG_ROUTING_NONE,
                                      MSG_ROUTING_NONE,
                                      FrameReplicationState(),
                                      false);
  controller_->delegate()->RenderFrameForInterstitialPageCreated(
      frame_tree_->root()->current_frame_host());
  view->SetSize(web_contents()->GetContainerBounds().size());
  view->Hide();
  return wcv;
}
