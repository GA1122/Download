static Frame* CreateNewWindow(LocalFrame& opener_frame,
                              const FrameLoadRequest& request,
                              const WebWindowFeatures& features,
                              NavigationPolicy policy,
                              bool& created) {
  Page* old_page = opener_frame.GetPage();
  if (!old_page)
    return nullptr;

  policy = EffectiveNavigationPolicy(policy, WebViewImpl::CurrentInputEvent(),
                                     features);

  const SandboxFlags sandbox_flags =
      opener_frame.GetDocument()->IsSandboxed(
          kSandboxPropagatesToAuxiliaryBrowsingContexts)
          ? opener_frame.GetSecurityContext()->GetSandboxFlags()
          : kSandboxNone;

  Page* page = old_page->GetChromeClient().CreateWindow(
      &opener_frame, request, features, policy, sandbox_flags);
  if (!page)
    return nullptr;

  if (page == old_page)
    return &opener_frame.Tree().Top();

  DCHECK(page->MainFrame());
  LocalFrame& frame = *ToLocalFrame(page->MainFrame());

  page->SetWindowFeatures(features);

  frame.View()->SetCanHaveScrollbars(features.scrollbars_visible);


  IntRect window_rect = page->GetChromeClient().RootWindowRect();
  IntSize viewport_size = page->GetChromeClient().PageRect().Size();

  if (features.x_set)
    window_rect.SetX(features.x);
  if (features.y_set)
    window_rect.SetY(features.y);
  if (features.width_set)
    window_rect.SetWidth(features.width +
                         (window_rect.Width() - viewport_size.Width()));
  if (features.height_set)
    window_rect.SetHeight(features.height +
                          (window_rect.Height() - viewport_size.Height()));

  page->GetChromeClient().SetWindowRectWithAdjustment(window_rect, frame);
  page->GetChromeClient().Show(policy);

  probe::windowCreated(&opener_frame, &frame);
  created = true;
  return &frame;
}
