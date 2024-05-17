  SparseAttributeAXPropertyAdapter(AXObject& axObject,
                                   protocol::Array<AXProperty>& properties)
      : m_axObject(&axObject), m_properties(properties) {}
