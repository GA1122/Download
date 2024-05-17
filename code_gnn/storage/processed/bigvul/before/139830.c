void Shell::HideShelf() {
  RootWindowControllerList controllers = GetAllRootWindowControllers();
  for (RootWindowControllerList::iterator iter = controllers.begin();
       iter != controllers.end(); ++iter) {
    if ((*iter)->shelf())
      (*iter)->shelf()->ShutdownStatusAreaWidget();
  }
}
