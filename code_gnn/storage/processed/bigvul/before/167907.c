bool LocalFrame::CanNavigate(const Frame& target_frame,
                             const KURL& destination_url) {
  String error_reason;
  const bool is_allowed_navigation =
      CanNavigateWithoutFramebusting(target_frame, error_reason);
  const bool sandboxed =
      GetSecurityContext()->GetSandboxFlags() != kSandboxNone;
  const bool has_user_gesture = HasBeenActivated();

  if (target_frame != this && sandboxed && target_frame == Tree().Top()) {
    UseCounter::Count(this, WebFeature::kTopNavInSandbox);
    if (!has_user_gesture) {
      UseCounter::Count(this, WebFeature::kTopNavInSandboxWithoutGesture);
    }
  }

  if (target_frame != this &&
      !GetSecurityContext()->IsSandboxed(kSandboxTopNavigation) &&
      target_frame == Tree().Top()) {
    DEFINE_STATIC_LOCAL(EnumerationHistogram, framebust_histogram,
                        ("WebCore.Framebust", 4));
    const unsigned kUserGestureBit = 0x1;
    const unsigned kAllowedBit = 0x2;
    unsigned framebust_params = 0;

    if (has_user_gesture)
      framebust_params |= kUserGestureBit;

    UseCounter::Count(this, WebFeature::kTopNavigationFromSubFrame);
    if (sandboxed) {   
      UseCounter::Count(this, WebFeature::kTopNavInSandboxWithPerm);
      if (!has_user_gesture) {
        UseCounter::Count(this,
                          WebFeature::kTopNavInSandboxWithPermButNoGesture);
      }
    }

    if (is_allowed_navigation)
      framebust_params |= kAllowedBit;
    framebust_histogram.Count(framebust_params);
    if (has_user_gesture || is_allowed_navigation ||
        target_frame.GetSecurityContext()->GetSecurityOrigin()->CanAccess(
            SecurityOrigin::Create(destination_url).get())) {
      return true;
    }
    if (!RuntimeEnabledFeatures::
            FramebustingNeedsSameOriginOrUserGestureEnabled() ||
        Client()->GetContentSettingsClient().AllowPopupsAndRedirects(
            false  )) {
      String target_frame_description =
          target_frame.IsLocalFrame() ? "with URL '" +
                                            ToLocalFrame(target_frame)
                                                .GetDocument()
                                                ->Url()
                                                .GetString() +
                                            "'"
                                      : "with origin '" +
                                            target_frame.GetSecurityContext()
                                                ->GetSecurityOrigin()
                                                ->ToString() +
                                            "'";
      String message = "Frame with URL '" + GetDocument()->Url().GetString() +
                       "' attempted to navigate its top-level window " +
                       target_frame_description +
                       ". Navigating the top-level window from a cross-origin "
                       "iframe will soon require that the iframe has received "
                       "a user gesture. See "
                       "https://www.chromestatus.com/features/"
                       "5851021045661696.";
      PrintNavigationWarning(message);
      return true;
    }
    error_reason =
        "The frame attempting navigation is targeting its top-level window, "
        "but is neither same-origin with its target nor has it received a "
        "user gesture. See "
        "https://www.chromestatus.com/features/5851021045661696.";
    PrintNavigationErrorMessage(target_frame, error_reason.Latin1().data());
    Client()->DidBlockFramebust(destination_url);
    return false;
  }

  if (Client()->Opener() == target_frame &&
      !HasTransientUserActivation(this, false  ) &&
      !target_frame.GetSecurityContext()->GetSecurityOrigin()->CanAccess(
          SecurityOrigin::Create(destination_url).get())) {
    UseCounter::Count(this, WebFeature::kOpenerNavigationWithoutGesture);
  }

  if (!is_allowed_navigation && !error_reason.IsNull())
    PrintNavigationErrorMessage(target_frame, error_reason.Latin1().data());
  return is_allowed_navigation;
}
