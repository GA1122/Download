void OutOfProcessInstance::PrintEnd() {
  if (print_settings_.print_pages_called_)
    UserMetricsRecordAction("PDF.PrintPage");
  print_settings_.Clear();
  engine_->PrintEnd();
}
