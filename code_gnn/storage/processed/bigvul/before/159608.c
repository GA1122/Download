Node* Document::importNode(Node* imported_node,
                           bool deep,
                           ExceptionState& exception_state) {
  switch (imported_node->getNodeType()) {
    case kTextNode:
      return createTextNode(imported_node->nodeValue());
    case kCdataSectionNode:
      return CDATASection::Create(*this, imported_node->nodeValue());
    case kProcessingInstructionNode:
      return createProcessingInstruction(imported_node->nodeName(),
                                         imported_node->nodeValue(),
                                         exception_state);
    case kCommentNode:
      return createComment(imported_node->nodeValue());
    case kDocumentTypeNode: {
      DocumentType* doctype = ToDocumentType(imported_node);
      return DocumentType::Create(this, doctype->name(), doctype->publicId(),
                                  doctype->systemId());
    }
    case kElementNode: {
      Element* old_element = ToElement(imported_node);
      if (!HasValidNamespaceForElements(old_element->TagQName())) {
        exception_state.ThrowDOMException(
            kNamespaceError, "The imported node has an invalid namespace.");
        return nullptr;
      }
      Element* new_element =
          createElement(old_element->TagQName(), kCreatedByImportNode);

      new_element->CloneDataFromElement(*old_element);

      if (deep) {
        if (!ImportContainerNodeChildren(old_element, new_element,
                                         exception_state))
          return nullptr;
        if (IsHTMLTemplateElement(*old_element) &&
            !EnsureTemplateDocument().ImportContainerNodeChildren(
                ToHTMLTemplateElement(old_element)->content(),
                ToHTMLTemplateElement(new_element)->content(), exception_state))
          return nullptr;
      }

      return new_element;
    }
    case kAttributeNode:
      return Attr::Create(
          *this,
          QualifiedName(g_null_atom,
                        AtomicString(ToAttr(imported_node)->name()),
                        g_null_atom),
          ToAttr(imported_node)->value());
    case kDocumentFragmentNode: {
      if (imported_node->IsShadowRoot()) {
        exception_state.ThrowDOMException(
            kNotSupportedError,
            "The node provided is a shadow root, which may not be imported.");
        return nullptr;
      }
      DocumentFragment* old_fragment = ToDocumentFragment(imported_node);
      DocumentFragment* new_fragment = createDocumentFragment();
      if (deep && !ImportContainerNodeChildren(old_fragment, new_fragment,
                                               exception_state))
        return nullptr;

      return new_fragment;
    }
    case kDocumentNode:
      exception_state.ThrowDOMException(
          kNotSupportedError,
          "The node provided is a document, which may not be imported.");
      return nullptr;
  }

  NOTREACHED();
  return nullptr;
}
