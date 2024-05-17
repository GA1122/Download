void PaymentHandlerWebFlowViewController::DidStartNavigation(
    content::NavigationHandle* navigation_handle) {
  if (navigation_handle->IsSameDocument())
    return;

  UpdateHeaderView();
}
