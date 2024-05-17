void CSSStyleSheet::Trace(blink::Visitor* visitor) {
  visitor->Trace(contents_);
  visitor->Trace(owner_node_);
  visitor->Trace(owner_rule_);
  visitor->Trace(constructed_tree_scopes_);
  visitor->Trace(media_cssom_wrapper_);
  visitor->Trace(child_rule_cssom_wrappers_);
  visitor->Trace(rule_list_cssom_wrapper_);
  StyleSheet::Trace(visitor);
}
