void PeopleHandler::OnPrimaryAccountSet(
    const AccountInfo& primary_account_info) {
  UpdateSyncStatus();
}
