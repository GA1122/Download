void RenderFrameImpl::didCommitProvisionalLoad(
    blink::WebLocalFrame* frame,
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type) {
  TRACE_EVENT2("navigation", "RenderFrameImpl::didCommitProvisionalLoad",
               "id", routing_id_,
               "url", GetLoadingUrl().possibly_invalid_spec());
  DCHECK(!frame_ || frame_ == frame);
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());

  if (proxy_routing_id_ != MSG_ROUTING_NONE) {
    RenderFrameProxy* proxy =
        RenderFrameProxy::FromRoutingID(proxy_routing_id_);
    CHECK(proxy);
    proxy->web_frame()->swap(frame_);
    proxy_routing_id_ = MSG_ROUTING_NONE;

    if (!is_subframe_) {
      CHECK(!render_view_->main_render_frame_);
      render_view_->main_render_frame_ = this;
      if (render_view_->is_swapped_out())
        render_view_->SetSwappedOut(false);
    }
  }

  render_view_->SendUpdateState();

  render_view_->history_controller()->UpdateForCommit(
      this, item, commit_type, navigation_state->WasWithinSamePage());

  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  if (document_state->commit_load_time().is_null())
    document_state->set_commit_load_time(Time::Now());

  if (internal_data->must_reset_scroll_and_scale_state()) {
    render_view_->webview()->resetScrollAndScaleState();
    internal_data->set_must_reset_scroll_and_scale_state(false);
  }

  bool is_new_navigation = commit_type == blink::WebStandardCommit;
  if (is_new_navigation) {
    render_view_->page_id_ = render_view_->next_page_id_++;

    DCHECK_IMPLIES(
        navigation_state->common_params().should_replace_current_entry,
        render_view_->history_list_length_ > 0);
    if (GetLoadingUrl() != GURL(kSwappedOutURL) &&
        !navigation_state->common_params().should_replace_current_entry) {
      render_view_->history_list_offset_++;
      if (render_view_->history_list_offset_ >= kMaxSessionHistoryEntries)
        render_view_->history_list_offset_ = kMaxSessionHistoryEntries - 1;
      render_view_->history_list_length_ =
          render_view_->history_list_offset_ + 1;
    }
  } else {
    const RequestNavigationParams& request_params =
        navigation_state->request_params();
    if (request_params.nav_entry_id != 0 &&
        !request_params.intended_as_new_entry) {
      render_view_->page_id_ = request_params.page_id;

      render_view_->history_list_offset_ =
          request_params.pending_history_list_offset;
    }
  }

  bool sent = Send(
      new FrameHostMsg_DidAssignPageId(routing_id_, render_view_->page_id_));
  CHECK(sent);   

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers_,
                    DidCommitProvisionalLoad(frame, is_new_navigation));
  FOR_EACH_OBSERVER(
      RenderFrameObserver, observers_,
      DidCommitProvisionalLoad(is_new_navigation,
                               navigation_state->WasWithinSamePage()));

  if (!frame->parent()) {   
    RenderThreadImpl* render_thread_impl = RenderThreadImpl::current();
    if (render_thread_impl) {   
      render_thread_impl->histogram_customizer()->
          RenderViewNavigatedToHost(GURL(GetLoadingUrl()).host(),
                                    RenderView::GetRenderViewCount());
    }
  }

  navigation_state->set_request_committed(true);

  SendDidCommitProvisionalLoad(frame, commit_type, item);

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
