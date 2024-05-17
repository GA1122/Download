void PeopleHandler::OnAccountImageUpdated(const std::string& account_id,
                                          const gfx::Image& image) {
  FireWebUIListener("stored-accounts-updated", *GetStoredAccountsList());
}
