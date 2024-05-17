  void LogInAsChild(const AccountId& account_id,
                    const std::string& user_id_hash) {
    SessionManager::Get()->CreateSession(account_id, user_id_hash);
    user_manager::User* user =
        user_manager::UserManager::Get()->FindUserAndModify(account_id);
    user_manager::UserManager::Get()->ChangeUserChildStatus(
        user, true  );
  }
