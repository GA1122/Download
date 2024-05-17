bool AccessibilityTreeContainsNodeWithName(BrowserAccessibility* node,
                                           const std::string& name) {
  if (node->GetStringAttribute(ax::mojom::StringAttribute::kName) == name)
    return true;
  for (unsigned i = 0; i < node->PlatformChildCount(); i++) {
    if (AccessibilityTreeContainsNodeWithName(node->PlatformGetChild(i), name))
      return true;
  }
  return false;
}
