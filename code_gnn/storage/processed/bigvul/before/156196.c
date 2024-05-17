const char* RendererSchedulerImpl::ExpensiveTaskPolicyToString(
    ExpensiveTaskPolicy expensive_task_policy) {
  switch (expensive_task_policy) {
    case ExpensiveTaskPolicy::kRun:
      return "run";
    case ExpensiveTaskPolicy::kBlock:
      return "block";
    case ExpensiveTaskPolicy::kThrottle:
      return "throttle";
    default:
      NOTREACHED();
      return nullptr;
  }
}
