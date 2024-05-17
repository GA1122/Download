bool LocalFrame::CanNavigateWithoutFramebusting(const Frame& target_frame,
                                                String& reason) {
  if (&target_frame == this)
    return true;

  if (GetSecurityContext()->IsSandboxed(kSandboxNavigation)) {
    if (!target_frame.Tree().IsDescendantOf(this) &&
        !target_frame.IsMainFrame()) {
      reason =
          "The frame attempting navigation is sandboxed, and is therefore "
          "disallowed from navigating its ancestors.";
      return false;
    }

    if (target_frame.IsMainFrame() && target_frame != Tree().Top() &&
        GetSecurityContext()->IsSandboxed(
            kSandboxPropagatesToAuxiliaryBrowsingContexts) &&
        (GetSecurityContext()->IsSandboxed(kSandboxPopups) ||
         target_frame.Client()->Opener() != this)) {
      reason =
          "The frame attempting navigation is sandboxed and is trying "
          "to navigate a popup, but is not the popup's opener and is not "
          "set to propagate sandboxing to popups.";
      return false;
    }

    if (target_frame == Tree().Top()) {
      if (GetSecurityContext()->IsSandboxed(kSandboxTopNavigation) &&
          GetSecurityContext()->IsSandboxed(
              kSandboxTopNavigationByUserActivation)) {
        reason =
            "The frame attempting navigation of the top-level window is "
            "sandboxed, but the flag of 'allow-top-navigation' or "
            "'allow-top-navigation-by-user-activation' is not set.";
        return false;
      }
      if (GetSecurityContext()->IsSandboxed(kSandboxTopNavigation) &&
          !GetSecurityContext()->IsSandboxed(
              kSandboxTopNavigationByUserActivation) &&
          !Frame::HasTransientUserActivation(this)) {
        reason =
            "The frame attempting navigation of the top-level window is "
            "sandboxed with the 'allow-top-navigation-by-user-activation' "
            "flag, but has no user activation (aka gesture). See "
            "https://www.chromestatus.com/feature/5629582019395584.";
        return false;
      }
      return true;
    }
  }

  DCHECK(GetSecurityContext()->GetSecurityOrigin());
  const SecurityOrigin& origin = *GetSecurityContext()->GetSecurityOrigin();

  if (CanAccessAncestor(origin, &target_frame))
    return true;

  if (!target_frame.Tree().Parent()) {
    if (target_frame == Client()->Opener())
      return true;
    if (CanAccessAncestor(origin, target_frame.Client()->Opener()))
      return true;
  }

  reason =
      "The frame attempting navigation is neither same-origin with the target, "
      "nor is it the target's parent or opener.";
  return false;
}
