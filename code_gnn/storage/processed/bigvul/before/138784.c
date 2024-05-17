void RenderFrameHostImpl::ReportContentSecurityPolicyViolation(
    const CSPViolationParams& violation_params) {
  Send(new FrameMsg_ReportContentSecurityPolicyViolation(routing_id_,
                                                         violation_params));
}
