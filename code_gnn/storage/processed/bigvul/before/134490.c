RenderWidgetHostView* WebContentsViewAura::CreateViewForWidget(
    RenderWidgetHost* render_widget_host) {
  if (render_widget_host->GetView()) {
    DCHECK(RenderViewHostFactory::has_factory());
    return render_widget_host->GetView();
  }

  RenderWidgetHostView* view =
      RenderWidgetHostView::CreateViewForWidget(render_widget_host);
  view->InitAsChild(NULL);
  GetNativeView()->AddChild(view->GetNativeView());

  if (navigation_overlay_.get() && navigation_overlay_->has_window()) {
    navigation_overlay_->StartObservingView(ToRenderWidgetHostViewAura(view));
  }

  RenderWidgetHostImpl* host_impl =
      RenderWidgetHostImpl::From(render_widget_host);

  if (!host_impl->is_hidden())
    view->Show();

  aura::client::SetDragDropDelegate(view->GetNativeView(), this);

  if (host_impl->overscroll_controller() &&
      (!web_contents_->GetDelegate() ||
       web_contents_->GetDelegate()->CanOverscrollContent())) {
    host_impl->overscroll_controller()->set_delegate(this);
    if (!navigation_overlay_)
      navigation_overlay_.reset(new OverscrollNavigationOverlay(web_contents_));
  }

  AttachTouchEditableToRenderView();
  return view;
}
