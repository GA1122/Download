const char* RendererSchedulerImpl::TimeDomainTypeToString(
    TimeDomainType domain_type) {
  switch (domain_type) {
    case TimeDomainType::kReal:
      return "real";
    case TimeDomainType::kThrottled:
      return "throttled";
    case TimeDomainType::kVirtual:
      return "virtual";
    default:
      NOTREACHED();
      return nullptr;
  }
}
