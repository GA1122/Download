bool NavigationController::IsRedirect(
  const ViewHostMsg_FrameNavigate_Params& params) {
  if (PageTransition::IsMainFrame(params.transition)) {
    return PageTransition::IsRedirect(params.transition);
  }
  return params.redirects.size() > 1;
}
