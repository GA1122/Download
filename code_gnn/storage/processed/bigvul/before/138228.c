void AXObjectCacheImpl::didShowMenuListPopup(LayoutMenuList* menuList) {
  AXObject* obj = get(menuList);
  if (!obj || !obj->isMenuList())
    return;

  toAXMenuList(obj)->didShowPopup();
}
