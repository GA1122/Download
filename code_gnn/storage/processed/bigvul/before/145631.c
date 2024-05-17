void NavigationRequest::OnRequestFailed(
    const network::URLLoaderCompletionStatus& status) {
  bool collapse_frame =
      status.extended_error_code ==
      static_cast<int>(blink::ResourceRequestBlockedReason::kCollapsedByClient);
  OnRequestFailedInternal(status, false  ,
                          base::nullopt  ,
                          collapse_frame);
}
