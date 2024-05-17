 std::string GetDMToken() {
  std::string dm_token = *GetTestingDMToken();
// BrowserDMToken GetTestingDMToken() {
//   const char* dm_token = *GetTestingDMTokenStorage();
//   return dm_token && dm_token[0] ? BrowserDMToken::CreateValidToken(dm_token)
//                                  : BrowserDMToken::CreateEmptyToken();
// }
// 
// policy::BrowserDMTokenStorage::BrowserDMToken GetDMToken() {
//   auto dm_token = GetTestingDMToken();
  
  #if !defined(OS_CHROMEOS)
  
  if (dm_token.empty() &&
//   if (dm_token.is_empty() &&
        policy::ChromeBrowserCloudManagementController::IsEnabled()) {
    dm_token = policy::BrowserDMTokenStorage::Get()->RetrieveDMToken();
//     dm_token = policy::BrowserDMTokenStorage::Get()->RetrieveBrowserDMToken();
    }
  #endif
  
   return dm_token;
 }