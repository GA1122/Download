void AXTree::CallNodeChangeCallbacks(AXNode* node,
                                     const AXNodeData& old_data,
                                     const AXNodeData& new_data) {
  for (AXTreeObserver& observer : observers_)
    observer.OnNodeDataWillChange(this, old_data, new_data);

  if (old_data.role != new_data.role) {
    for (AXTreeObserver& observer : observers_)
      observer.OnRoleChanged(this, node, old_data.role, new_data.role);
  }

  if (old_data.state != new_data.state) {
    for (int32_t i = static_cast<int32_t>(ax::mojom::State::kNone) + 1;
         i <= static_cast<int32_t>(ax::mojom::State::kMaxValue); ++i) {
      ax::mojom::State state = static_cast<ax::mojom::State>(i);
      if (old_data.HasState(state) != new_data.HasState(state)) {
        for (AXTreeObserver& observer : observers_)
          observer.OnStateChanged(this, node, state, new_data.HasState(state));
      }
    }
  }

  auto string_callback = [this, node](ax::mojom::StringAttribute attr,
                                      const std::string& old_string,
                                      const std::string& new_string) {
    for (AXTreeObserver& observer : observers_) {
      observer.OnStringAttributeChanged(this, node, attr, old_string,
                                        new_string);
    }
  };
  CallIfAttributeValuesChanged(old_data.string_attributes,
                               new_data.string_attributes, std::string(),
                               string_callback);

  auto bool_callback = [this, node](ax::mojom::BoolAttribute attr,
                                    const bool& old_bool,
                                    const bool& new_bool) {
    for (AXTreeObserver& observer : observers_)
      observer.OnBoolAttributeChanged(this, node, attr, new_bool);
  };
  CallIfAttributeValuesChanged(old_data.bool_attributes,
                               new_data.bool_attributes, false, bool_callback);

  auto float_callback = [this, node](ax::mojom::FloatAttribute attr,
                                     const float& old_float,
                                     const float& new_float) {
    for (AXTreeObserver& observer : observers_)
      observer.OnFloatAttributeChanged(this, node, attr, old_float, new_float);
  };
  CallIfAttributeValuesChanged(old_data.float_attributes,
                               new_data.float_attributes, 0.0f, float_callback);

  auto int_callback = [this, node](ax::mojom::IntAttribute attr,
                                   const int& old_int, const int& new_int) {
    for (AXTreeObserver& observer : observers_)
      observer.OnIntAttributeChanged(this, node, attr, old_int, new_int);
  };
  CallIfAttributeValuesChanged(old_data.int_attributes, new_data.int_attributes,
                               0, int_callback);

  auto intlist_callback = [this, node](
                              ax::mojom::IntListAttribute attr,
                              const std::vector<int32_t>& old_intlist,
                              const std::vector<int32_t>& new_intlist) {
    for (AXTreeObserver& observer : observers_)
      observer.OnIntListAttributeChanged(this, node, attr, old_intlist,
                                         new_intlist);
  };
  CallIfAttributeValuesChanged(old_data.intlist_attributes,
                               new_data.intlist_attributes,
                               std::vector<int32_t>(), intlist_callback);

  auto stringlist_callback =
      [this, node](ax::mojom::StringListAttribute attr,
                   const std::vector<std::string>& old_stringlist,
                   const std::vector<std::string>& new_stringlist) {
        for (AXTreeObserver& observer : observers_)
          observer.OnStringListAttributeChanged(this, node, attr,
                                                old_stringlist, new_stringlist);
      };
  CallIfAttributeValuesChanged(old_data.stringlist_attributes,
                               new_data.stringlist_attributes,
                               std::vector<std::string>(), stringlist_callback);
}
