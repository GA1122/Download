ShelfLayoutManager::~ShelfLayoutManager() {
  FOR_EACH_OBSERVER(Observer, observers_, WillDeleteShelf());
  Shell::GetInstance()->RemoveShellObserver(this);
  aura::client::GetActivationClient(root_window_)->RemoveObserver(this);
}
