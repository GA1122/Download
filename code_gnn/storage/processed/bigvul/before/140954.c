bool Document::AllowInlineEventHandler(Node* node,
                                       EventListener* listener,
                                       const String& context_url,
                                       const WTF::OrdinalNumber& context_line) {
  Element* element = node && node->IsElementNode() ? ToElement(node) : nullptr;
  if (!ContentSecurityPolicy::ShouldBypassMainWorld(this) &&
      !GetContentSecurityPolicy()->AllowInline(
          ContentSecurityPolicy::InlineType::kInlineEventHandler, element,
          listener->ScriptBody(), String()  , context_url,
          context_line))
    return false;

  LocalFrame* frame = ExecutingFrame();
  if (!frame)
    return false;
  if (!ContextDocument()->CanExecuteScripts(kNotAboutToExecuteScript))
    return false;
  if (node && node->GetDocument() != this &&
      !node->GetDocument().AllowInlineEventHandler(node, listener, context_url,
                                                   context_line))
    return false;

  return true;
}
