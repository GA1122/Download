void OnWildcardCheckCompleted(const std::string& username,
                              WildcardLoginChecker::Result result) {
  if (result == WildcardLoginChecker::RESULT_BLOCKED) {
    LOG(ERROR) << "Online wildcard login check failed, terminating session.";

    chromeos::UserManager::Get()->RemoveUserFromList(username);
    chrome::AttemptUserExit();
  }
}