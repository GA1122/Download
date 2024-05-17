bool Performance::AllowsTimingRedirect(
    const Vector<ResourceResponse>& redirect_chain,
    const ResourceResponse& final_response,
    const SecurityOrigin& initiator_security_origin,
    ExecutionContext* context) {
  if (!PassesTimingAllowCheck(final_response, initiator_security_origin,
                              AtomicString(), context))
    return false;

  for (const ResourceResponse& response : redirect_chain) {
    if (!PassesTimingAllowCheck(response, initiator_security_origin,
                                AtomicString(), context))
      return false;
  }

  return true;
}
