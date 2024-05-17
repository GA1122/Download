  void OnUserInteractionFor(WebContents* web_contents,
                            blink::WebInputEvent::Type type) {
    DownloadRequestLimiter::TabDownloadState* state =
        download_request_limiter_->GetDownloadState(web_contents, nullptr,
                                                    false);
    if (state)
      state->DidGetUserInteraction(type);
  }
