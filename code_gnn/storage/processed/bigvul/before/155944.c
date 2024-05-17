void PeopleHandler::OnAccountUpdated(const AccountInfo& info) {
  FireWebUIListener("stored-accounts-updated", *GetStoredAccountsList());
}
