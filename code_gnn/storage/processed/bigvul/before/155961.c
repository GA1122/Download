PeopleHandler::~PeopleHandler() {
  if (!web_ui())
    return;

  CloseSyncSetup();
}
