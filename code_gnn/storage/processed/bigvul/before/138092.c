AXObject* AXNodeObject::menuButtonForMenu() const {
  Element* menuItem = menuItemElementForMenu();

  if (menuItem) {
    AXObject* menuItemAX = axObjectCache().getOrCreate(menuItem);
    if (menuItemAX && menuItemAX->isMenuButton())
      return menuItemAX;
  }
  return 0;
}
