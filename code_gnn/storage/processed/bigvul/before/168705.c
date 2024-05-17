bool PerformanceNavigationTiming::GetAllowRedirectDetails() const {
  ExecutionContext* context = GetFrame() ? GetFrame()->GetDocument() : nullptr;
  const SecurityOrigin* security_origin = nullptr;
  if (context)
    security_origin = context->GetSecurityOrigin();
  if (!security_origin)
    return false;
  return Performance::AllowsTimingRedirect(
      resource_timing_info_->RedirectChain(),
      resource_timing_info_->FinalResponse(), *security_origin, context);
}
