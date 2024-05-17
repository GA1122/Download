  void LogIn(const AccountId& account_id, const std::string& user_id_hash) {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    SessionManager::Get()->CreateSession(account_id, user_id_hash);
    SessionManager::Get()->SessionStarted();
    SessionControllerClient::FlushForTesting();
    WaitAsyncWallpaperLoadStarted();
  }
