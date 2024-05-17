std::string TreeToStringHelper(AXNode* node, int indent) {
  std::string result = std::string(2 * indent, ' ');
  result += node->data().ToString() + "\n";
  for (int i = 0; i < node->child_count(); ++i)
    result += TreeToStringHelper(node->ChildAtIndex(i), indent + 1);
  return result;
}
