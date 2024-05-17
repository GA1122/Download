void Performance::measure(const String& measure_name,
                          const String& start_mark,
                          const String& end_mark,
                          ExceptionState& exception_state) {
  UMA_HISTOGRAM_ENUMERATION(
      "Performance.PerformanceMeasurePassedInParameter.StartMark",
      ToPerformanceMeasurePassedInParameterType(start_mark),
      kPerformanceMeasurePassedInParameterCount);
  UMA_HISTOGRAM_ENUMERATION(
      "Performance.PerformanceMeasurePassedInParameter.EndMark",
      ToPerformanceMeasurePassedInParameterType(end_mark),
      kPerformanceMeasurePassedInParameterCount);

  ExecutionContext* execution_context = GetExecutionContext();
  if (execution_context) {
    PerformanceMeasurePassedInParameterType start_type =
        ToPerformanceMeasurePassedInParameterType(start_mark);
    PerformanceMeasurePassedInParameterType end_type =
        ToPerformanceMeasurePassedInParameterType(end_mark);

    if (start_type == kObjectObject) {
      UseCounter::Count(execution_context,
                        WebFeature::kPerformanceMeasurePassedInObject);
    }

    if (IsNavigationTimingType(start_type) ||
        IsNavigationTimingType(end_type)) {
      UseCounter::Count(
          execution_context,
          WebFeature::kPerformanceMeasurePassedInNavigationTiming);
    }
  }

  if (!user_timing_)
    user_timing_ = UserTiming::Create(*this);
  if (PerformanceEntry* entry = user_timing_->Measure(
          measure_name, start_mark, end_mark, exception_state))
    NotifyObserversOfEntry(*entry);
}
