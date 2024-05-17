void OutOfProcessInstance::UserMetricsRecordAction(
    const std::string& action) {
  pp::PDF::UserMetricsRecordAction(this, pp::Var(action));
}
