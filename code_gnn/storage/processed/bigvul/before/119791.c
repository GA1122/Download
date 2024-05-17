bool NavigationControllerImpl::IsRedirect(
  const ViewHostMsg_FrameNavigate_Params& params) {
  if (PageTransitionIsMainFrame(params.transition)) {
    return PageTransitionIsRedirect(params.transition);
  }
  return params.redirects.size() > 1;
}
