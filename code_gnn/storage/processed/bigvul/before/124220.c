string16 ExpectedTitleFromAuth(const string16& username,
                               const string16& password) {
  return username + UTF8ToUTF16("/") + password;
}
