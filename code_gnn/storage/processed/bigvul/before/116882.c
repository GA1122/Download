long long TestWebKitPlatformSupport::databaseGetSpaceAvailableForOrigin(
    const WebKit::WebString& origin_identifier) {
  return SimpleDatabaseSystem::GetInstance()->GetSpaceAvailable(
      origin_identifier);
}
