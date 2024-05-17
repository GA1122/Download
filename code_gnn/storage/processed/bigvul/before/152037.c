void RenderFrameHostImpl::OnOpenURL(const FrameHostMsg_OpenURL_Params& params) {
  GURL validated_url;
  scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory;
  if (!VerifyOpenURLParams(GetSiteInstance(), params, &validated_url,
                           &blob_url_loader_factory)) {
    return;
  }

  if (params.is_history_navigation_in_new_child) {
    if (frame_tree_node_->navigator()->StartHistoryNavigationInNewSubframe(
            this, validated_url)) {
      return;
    }
  }

  TRACE_EVENT1("navigation", "RenderFrameHostImpl::OpenURL", "url",
               validated_url.possibly_invalid_spec());

  frame_tree_node_->navigator()->RequestOpenURL(
      this, validated_url, params.initiator_origin, params.uses_post,
      params.resource_request_body, params.extra_headers, params.referrer,
      params.disposition, params.should_replace_current_entry,
      params.user_gesture, params.triggering_event_info, params.href_translate,
      std::move(blob_url_loader_factory));
}
