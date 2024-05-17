 std::string* GetTestingDMToken() {
  static std::string dm_token;
// const char** GetTestingDMTokenStorage() {
//   static const char* dm_token = "";
    return &dm_token;
  }