void RenderFrameImpl::UpdateStateForCommit(
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type) {
  DocumentState* document_state =
      DocumentState::FromDocumentLoader(frame_->GetDocumentLoader());
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  bool is_new_navigation = UpdateNavigationHistory(item, commit_type);
  NotifyObserversOfNavigationCommit(is_new_navigation,
                                    navigation_state->WasWithinSameDocument());

  if (internal_data->must_reset_scroll_and_scale_state()) {
    render_view_->webview()->ResetScrollAndScaleState();
    internal_data->set_must_reset_scroll_and_scale_state(false);
  }
  UpdateZoomLevel();

  if (!frame_->Parent()) {   
    RenderThreadImpl* render_thread_impl = RenderThreadImpl::current();
    if (render_thread_impl) {   
      render_thread_impl->histogram_customizer()->RenderViewNavigatedToHost(
          GURL(GetLoadingUrl()).host(), RenderView::GetRenderViewCount());
    }
  }

  navigation_state->set_request_committed(true);

  if (url::Origin(frame_->GetSecurityOrigin()) == engagement_level_.first) {
    frame_->SetEngagementLevel(engagement_level_.second);
    engagement_level_.first = url::Origin();
  }

  if (url::Origin(frame_->GetSecurityOrigin()) ==
      high_media_engagement_origin_) {
    frame_->SetHasHighMediaEngagement(true);
    high_media_engagement_origin_ = url::Origin();
  }
}
