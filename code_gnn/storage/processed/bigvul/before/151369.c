void SetNodeInfo(TracedValue* value,
                 Node* node,
                 const char* id_field_name,
                 const char* name_field_name = nullptr) {
  value->SetIntegerWithCopiedName(id_field_name, DOMNodeIds::IdForNode(node));
  if (name_field_name)
    value->SetStringWithCopiedName(name_field_name, node->DebugName());
}
