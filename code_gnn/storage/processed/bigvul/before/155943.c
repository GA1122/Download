void PeopleHandler::OnAccountRemoved(const AccountInfo& info) {
  FireWebUIListener("stored-accounts-updated", *GetStoredAccountsList());
}
