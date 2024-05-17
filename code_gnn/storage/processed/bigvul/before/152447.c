void RenderFrameImpl::OnReportContentSecurityPolicyViolation(
    const content::CSPViolationParams& violation_params) {
  frame_->ReportContentSecurityPolicyViolation(
      BuildWebContentSecurityPolicyViolation(violation_params));
}
