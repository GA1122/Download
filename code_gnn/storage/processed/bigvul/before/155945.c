void PeopleHandler::OnDidClosePage(const base::ListValue* args) {
  if (!args->GetList()[0].GetBool() && !IsProfileAuthNeededOrHasErrors()) {
    MarkFirstSetupComplete();
  }

  CloseSyncSetup();
}
