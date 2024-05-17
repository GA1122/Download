bool LocalFrame::IsUsingDataSavingPreview() const {
  if (!Client())
    return false;

  WebURLRequest::PreviewsState previews_state =
      Client()->GetPreviewsStateForFrame();
  return previews_state &
         (WebURLRequest::kServerLoFiOn | WebURLRequest::kClientLoFiOn |
          WebURLRequest::kNoScriptOn);
}
