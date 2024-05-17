  void WaitForHistory() {
    BlockUntilHistoryProcessesPendingRequests(history_service());
  }
