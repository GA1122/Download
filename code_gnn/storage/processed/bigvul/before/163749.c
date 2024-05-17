static Frame* CreateWindowHelper(LocalFrame& opener_frame,
                                 LocalFrame& active_frame,
                                 LocalFrame& lookup_frame,
                                 const FrameLoadRequest& request,
                                 const WebWindowFeatures& features,
                                 NavigationPolicy policy,
                                 bool& created) {
  DCHECK(request.GetResourceRequest().RequestorOrigin() ||
         opener_frame.GetDocument()->Url().IsEmpty());
  DCHECK_EQ(request.GetResourceRequest().GetFrameType(),
            WebURLRequest::kFrameTypeAuxiliary);

  created = false;

  Frame* window =
      features.noopener
          ? nullptr
          : ReuseExistingWindow(active_frame, lookup_frame, request.FrameName(),
                                policy, request.GetResourceRequest().Url());

  if (!window) {
    if (opener_frame.GetDocument()->IsSandboxed(kSandboxPopups)) {
      opener_frame.GetDocument()->AddConsoleMessage(ConsoleMessage::Create(
          kSecurityMessageSource, kErrorMessageLevel,
          "Blocked opening '" +
              request.GetResourceRequest().Url().ElidedString() +
              "' in a new window because the request was made in a sandboxed "
              "frame whose 'allow-popups' permission is not set."));
      return nullptr;
    }
  }

  if (window) {
    if (!window->Client())
      return nullptr;
    if (request.GetShouldSetOpener() == kMaybeSetOpener)
      window->Client()->SetOpener(&opener_frame);
    return window;
  }

  return CreateNewWindow(opener_frame, request, features, policy, created);
}
