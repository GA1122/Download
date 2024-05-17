void SessionService::MoveCurrentSessionToLastSession() {
  pending_tab_close_ids_.clear();
  window_closing_ids_.clear();
  pending_window_close_ids_.clear();

  Save();

  RunTaskOnBackendThread(
      FROM_HERE, base::Bind(&SessionBackend::MoveCurrentSessionToLastSession,
                            backend()));
}
