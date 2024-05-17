void PeopleHandler::OnPrimaryAccountCleared(
    const AccountInfo& previous_primary_account_info) {
  UpdateSyncStatus();
}
