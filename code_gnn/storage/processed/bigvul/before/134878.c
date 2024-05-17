void WebPageSerializerImpl::buildContentForNode(Node* node,
                                                SerializeDomParam* param)
{
    switch (node->nodeType()) {
    case Node::ELEMENT_NODE:
        openTagToString(toElement(node), param);
        for (Node *child = node->firstChild(); child; child = child->nextSibling())
            buildContentForNode(child, param);
        endTagToString(toElement(node), param);
        break;
    case Node::TEXT_NODE:
        saveHTMLContentToBuffer(createMarkup(node), param);
        break;
    case Node::ATTRIBUTE_NODE:
    case Node::DOCUMENT_NODE:
    case Node::DOCUMENT_FRAGMENT_NODE:
        ASSERT_NOT_REACHED();
        break;
    case Node::DOCUMENT_TYPE_NODE:
        param->haveSeenDocType = true;
    default:
        saveHTMLContentToBuffer(createMarkup(node), param);
        break;
    }
}
