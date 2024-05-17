static void SetGeneratingNodeInfo(TracedValue* value,
                                  const LayoutObject* layout_object,
                                  const char* id_field_name,
                                  const char* name_field_name = nullptr) {
  Node* node = nullptr;
  for (; layout_object && !node; layout_object = layout_object->Parent())
    node = layout_object->GeneratingNode();
  if (!node)
    return;

  SetNodeInfo(value, node, id_field_name, name_field_name);
}
