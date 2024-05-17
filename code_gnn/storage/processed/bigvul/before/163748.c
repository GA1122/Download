void CreateWindowForRequest(const FrameLoadRequest& request,
                            LocalFrame& opener_frame,
                            NavigationPolicy policy) {
  DCHECK(request.GetResourceRequest().RequestorOrigin() ||
         (opener_frame.GetDocument() &&
          opener_frame.GetDocument()->Url().IsEmpty()));

  if (opener_frame.GetDocument()->PageDismissalEventBeingDispatched() !=
      Document::kNoDismissal)
    return;

  if (opener_frame.GetDocument() &&
      opener_frame.GetDocument()->IsSandboxed(kSandboxPopups))
    return;

  if (policy == kNavigationPolicyCurrentTab)
    policy = kNavigationPolicyNewForegroundTab;

  WebWindowFeatures features;
  features.noopener = request.GetShouldSetOpener() == kNeverSetOpener;
  bool created;
  Frame* new_frame =
      CreateWindowHelper(opener_frame, opener_frame, opener_frame, request,
                         features, policy, created);
  if (!new_frame)
    return;
  if (request.GetShouldSendReferrer() == kMaybeSendReferrer) {
    if (new_frame->IsLocalFrame())
      ToLocalFrame(new_frame)->GetDocument()->SetReferrerPolicy(
          opener_frame.GetDocument()->GetReferrerPolicy());
  }

  FrameLoadRequest new_request(0, request.GetResourceRequest());
  new_request.SetForm(request.Form());
  if (new_frame->IsLocalFrame())
    ToLocalFrame(new_frame)->Loader().Load(new_request);
}
