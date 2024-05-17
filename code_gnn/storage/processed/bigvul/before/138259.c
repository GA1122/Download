void AXObjectCacheImpl::handleUpdateActiveMenuOption(LayoutMenuList* menuList,
                                                     int optionIndex) {
  AXObject* obj = get(menuList);
  if (!obj || !obj->isMenuList())
    return;

  toAXMenuList(obj)->didUpdateActiveOption(optionIndex);
}
