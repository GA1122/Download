bool IsNavigationTimingType(
    Performance::PerformanceMeasurePassedInParameterType type) {
  return type != Performance::kObjectObject && type != Performance::kOther;
}
