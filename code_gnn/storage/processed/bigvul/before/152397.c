void RenderFrameImpl::LoadErrorPage(int reason) {
  LoadNavigationErrorPage(frame_->GetDocumentLoader(),
                          WebURLError(reason, frame_->GetDocument().Url()),
                          base::nullopt, true  ,
                          false  );
}
