  void OnUpdate(const ObjectPaintProperties::UpdateResult& result) {
    property_added_or_removed_ |= result.NewNodeCreated();
    property_changed_ |= !result.Unchanged();
  }
