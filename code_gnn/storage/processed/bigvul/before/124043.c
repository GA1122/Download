void BookmarksAPI::OnListenerAdded(const EventListenerInfo& details) {
  bookmark_event_router_.reset(new BookmarkEventRouter(
      BookmarkModelFactory::GetForProfile(profile_)));
  ExtensionSystem::Get(profile_)->event_router()->UnregisterObserver(this);
}
