  ~StubLogin() {
    chromeos::LoginUtils::Get()->DelegateDeleted(this);
  }
