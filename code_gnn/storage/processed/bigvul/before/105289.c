bool DOMPatchSupport::innerPatchNode(Digest* oldDigest, Digest* newDigest, ExceptionCode& ec)
{
    if (oldDigest->m_sha1 == newDigest->m_sha1)
        return true;

    Node* oldNode = oldDigest->m_node;
    Node* newNode = newDigest->m_node;

    if (newNode->nodeType() != oldNode->nodeType() || newNode->nodeName() != oldNode->nodeName())
        return m_domEditor->replaceChild(oldNode->parentNode(), newNode, oldNode, ec);

    if (oldNode->nodeValue() != newNode->nodeValue()) {
        if (!m_domEditor->setNodeValue(oldNode, newNode->nodeValue(), ec))
            return false;
    }

    if (oldNode->nodeType() != Node::ELEMENT_NODE)
        return true;

    Element* oldElement = static_cast<Element*>(oldNode);
    Element* newElement = static_cast<Element*>(newNode);
    if (oldDigest->m_attrsSHA1 != newDigest->m_attrsSHA1) {
        if (oldElement->hasAttributesWithoutUpdate()) {
            while (oldElement->attributeCount()) {
                Attribute* attr = oldElement->attributeItem(0);
                if (!m_domEditor->removeAttribute(oldElement, attr->localName(), ec))
                    return false;
            }
        }

        if (newElement->hasAttributesWithoutUpdate()) {
            size_t numAttrs = newElement->attributeCount();
            for (size_t i = 0; i < numAttrs; ++i) {
                const Attribute* attribute = newElement->attributeItem(i);
                if (!m_domEditor->setAttribute(oldElement, attribute->name().localName(), attribute->value(), ec))
                    return false;
            }
        }
    }

    bool result = innerPatchChildren(oldElement, oldDigest->m_children, newDigest->m_children, ec);
    m_unusedNodesMap.remove(newDigest->m_sha1);
    return result;
}
