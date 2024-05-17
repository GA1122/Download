void Job::OnError(int line_number, const base::string16& error) {
  HandleAlertOrError(false, line_number, error);
}
