void AeroPeekManager::DeleteAeroPeekWindow(int tab_id) {
  for (std::list<AeroPeekWindow*>::iterator i = tab_list_.begin();
       i != tab_list_.end(); ++i) {
    AeroPeekWindow* window = *i;
    if (window->tab_id() == tab_id) {
      tab_list_.erase(i);
      delete window;
      return;
    }
  }
}
