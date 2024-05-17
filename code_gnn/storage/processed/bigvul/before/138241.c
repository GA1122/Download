AXObject* AXObjectCacheImpl::getOrCreate(AccessibilityRole role) {
  AXObject* obj = nullptr;

  switch (role) {
    case ColumnRole:
      obj = AXTableColumn::create(*this);
      break;
    case TableHeaderContainerRole:
      obj = AXTableHeaderContainer::create(*this);
      break;
    case SliderThumbRole:
      obj = AXSliderThumb::create(*this);
      break;
    case MenuListPopupRole:
      obj = AXMenuListPopup::create(*this);
      break;
    case SpinButtonRole:
      obj = AXSpinButton::create(*this);
      break;
    case SpinButtonPartRole:
      obj = AXSpinButtonPart::create(*this);
      break;
    default:
      obj = nullptr;
  }

  if (!obj)
    return 0;

  getOrCreateAXID(obj);

  obj->init();
  return obj;
}
