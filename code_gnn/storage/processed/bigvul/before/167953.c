void LocalFrame::MaybeAllowImagePlaceholder(FetchParameters& params) const {
  if (GetSettings() && GetSettings()->GetFetchImagePlaceholders()) {
    params.SetAllowImagePlaceholder();
    return;
  }

  if (Client() &&
      ShouldUseClientLoFiForRequest(params.GetResourceRequest(),
                                    Client()->GetPreviewsStateForFrame())) {
    params.MutableResourceRequest().SetPreviewsState(
        params.GetResourceRequest().GetPreviewsState() |
        WebURLRequest::kClientLoFiOn);
    params.SetAllowImagePlaceholder();
  }
}
