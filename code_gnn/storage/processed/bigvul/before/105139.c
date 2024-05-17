PassRefPtr<Node> Range::processContentsBetweenOffsets(ActionType action, PassRefPtr<DocumentFragment> fragment,
    Node* container, unsigned startOffset, unsigned endOffset, ExceptionCode& ec)
{
    ASSERT(container);
    ASSERT(startOffset <= endOffset);

    RefPtr<Node> result;   
    switch (container->nodeType()) {
    case Node::TEXT_NODE:
    case Node::CDATA_SECTION_NODE:
    case Node::COMMENT_NODE:
        ASSERT(endOffset <= static_cast<CharacterData*>(container)->length());
        if (action == EXTRACT_CONTENTS || action == CLONE_CONTENTS) {
            RefPtr<CharacterData> c = static_pointer_cast<CharacterData>(container->cloneNode(true));
            deleteCharacterData(c, startOffset, endOffset, ec);
            if (fragment) {
                result = fragment;
                result->appendChild(c.release(), ec);
            } else
                result = c.release();
        }
        if (action == EXTRACT_CONTENTS || action == DELETE_CONTENTS)
            static_cast<CharacterData*>(container)->deleteData(startOffset, endOffset - startOffset, ec);
        break;
    case Node::PROCESSING_INSTRUCTION_NODE:
        ASSERT(endOffset <= static_cast<ProcessingInstruction*>(container)->data().length());
        if (action == EXTRACT_CONTENTS || action == CLONE_CONTENTS) {
            RefPtr<ProcessingInstruction> c = static_pointer_cast<ProcessingInstruction>(container->cloneNode(true));
            c->setData(c->data().substring(startOffset, endOffset - startOffset), ec);
            if (fragment) {
                result = fragment;
                result->appendChild(c.release(), ec);
            } else
                result = c.release();
        }
        if (action == EXTRACT_CONTENTS || action == DELETE_CONTENTS) {
            ProcessingInstruction* pi = static_cast<ProcessingInstruction*>(container);
            String data(pi->data());
            data.remove(startOffset, endOffset - startOffset);
            pi->setData(data, ec);
        }
        break;
    case Node::ELEMENT_NODE:
    case Node::ATTRIBUTE_NODE:
    case Node::ENTITY_REFERENCE_NODE:
    case Node::ENTITY_NODE:
    case Node::DOCUMENT_NODE:
    case Node::DOCUMENT_TYPE_NODE:
    case Node::DOCUMENT_FRAGMENT_NODE:
    case Node::NOTATION_NODE:
    case Node::XPATH_NAMESPACE_NODE:
        if (action == EXTRACT_CONTENTS || action == CLONE_CONTENTS) {
            if (fragment)
                result = fragment;
            else
                result = container->cloneNode(false);
        }

        Node* n = container->firstChild();
        Vector<RefPtr<Node> > nodes;
        for (unsigned i = startOffset; n && i; i--)
            n = n->nextSibling();
        for (unsigned i = startOffset; n && i < endOffset; i++, n = n->nextSibling())
            nodes.append(n);

        processNodes(action, nodes, container, result, ec);
        break;
    }

    return result.release();
}
