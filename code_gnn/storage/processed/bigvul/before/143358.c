void Job::Alert(const base::string16& message) {
  HandleAlertOrError(true, -1, message);
}
