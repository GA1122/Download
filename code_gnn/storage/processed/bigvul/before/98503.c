AeroPeekManager::~AeroPeekManager() {
  for (std::list<AeroPeekWindow*>::iterator i = tab_list_.begin();
       i != tab_list_.end(); ++i) {
    AeroPeekWindow* window = *i;
    delete window;
  }
}
