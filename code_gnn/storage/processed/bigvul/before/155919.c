void PeopleHandler::CloseUI() {
  CloseSyncSetup();
  FireWebUIListener("page-status-changed", base::Value(kDonePageStatus));
}
