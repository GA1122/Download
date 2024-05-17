bool ShouldSkipTab(const sessions::SessionTab& session_tab) {
    if (session_tab.navigations.empty())
      return true;

    int selected_index = session_tab.normalized_navigation_index();
    const sessions::SerializedNavigationEntry& current_navigation =
        session_tab.navigations.at(selected_index);

    if (current_navigation.virtual_url().is_empty())
      return true;

    return false;
}
