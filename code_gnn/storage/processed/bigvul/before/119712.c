void ResetScreenHandler::OnRollbackCheck(bool can_rollback) {
  VLOG(1) << "Callback from CanRollbackCheck, result " << can_rollback;
  rollback_available_ = can_rollback;
  ShowWithParams();
}
