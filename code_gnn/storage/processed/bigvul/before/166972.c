static bool IsAcceptableCSSStyleSheetParent(const Node& parent_node) {
  return parent_node.IsDocumentNode() || IsHTMLLinkElement(parent_node) ||
         IsHTMLStyleElement(parent_node) || IsSVGStyleElement(parent_node) ||
         parent_node.getNodeType() == Node::kProcessingInstructionNode;
}
