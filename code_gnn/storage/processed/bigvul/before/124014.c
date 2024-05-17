BookmarksAPI::BookmarksAPI(Profile* profile) : profile_(profile) {
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkCreated);
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkRemoved);
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkChanged);
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkMoved);
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkChildrenReordered);
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkImportBegan);
  ExtensionSystem::Get(profile_)->event_router()->RegisterObserver(
      this, keys::kOnBookmarkImportEnded);
}
