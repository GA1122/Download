bool RenderFrameHostImpl::CanCommitURL(const GURL& url) {
  if (IsRendererDebugURL(url))
    return false;


  if (!frame_tree_node()->IsMainFrame()) {
    RenderFrameHostImpl* main_frame =
        frame_tree_node()->frame_tree()->GetMainFrame();
    if (main_frame->is_mhtml_document()) {
      if (IsSameSiteInstance(main_frame))
        return true;

      base::debug::SetCrashKeyString(
          base::debug::AllocateCrashKeyString(
              "oopif_in_mhtml_page", base::debug::CrashKeySize::Size32),
          is_mhtml_document() ? "is_mhtml_doc" : "not_mhtml_doc");
      return false;
    }
  }

  if (!GetContentClient()->browser()->CanCommitURL(GetProcess(), url))
    return false;

  auto* policy = ChildProcessSecurityPolicyImpl::GetInstance();
  if (url.IsStandard() &&
      !policy->CanAccessDataForOrigin(GetProcess()->GetID(), url)) {
    return false;
  }

  return true;
}
