void WebContentsImpl::WasShown() {
  controller_.SetActive(true);

  if (auto* view = GetRenderWidgetHostView()) {
    view->Show();
#if defined(OS_MACOSX)
    view->SetActive(true);
#endif
  }

  if (!ShowingInterstitialPage())
    SetVisibilityForChildViews(true);

  SendPageMessage(new PageMsg_WasShown(MSG_ROUTING_NONE));

  last_active_time_ = base::TimeTicks::Now();
  SetVisibility(Visibility::VISIBLE);

  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    RenderFrameProxyHost* parent = node->render_manager()->GetProxyToParent();
    if (!parent)
      continue;

    if (parent->cross_process_frame_connector()->IsVisible()) {
      parent->cross_process_frame_connector()->MaybeLogCrash(
          CrossProcessFrameConnector::CrashVisibility::kShownAfterCrashing);
    }
  }
}
