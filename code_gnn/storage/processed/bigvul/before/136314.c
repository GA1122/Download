  void OnClear(bool cleared) {
    property_added_or_removed_ |= cleared;
    property_changed_ |= cleared;
  }
