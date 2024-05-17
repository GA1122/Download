RenderWidgetHostView* WebContentsImpl::GetTopLevelRenderWidgetHostView() {
  if (GetOuterWebContents())
    return GetOuterWebContents()->GetTopLevelRenderWidgetHostView();
  return GetRenderManager()->GetRenderWidgetHostView();
}
