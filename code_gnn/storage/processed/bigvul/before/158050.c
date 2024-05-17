void LocalFrameClientImpl::DidDetectXSS(const KURL& insecure_url,
                                        bool did_block_entire_page) {
  if (web_frame_->Client())
    web_frame_->Client()->DidDetectXSS(insecure_url, did_block_entire_page);
}
