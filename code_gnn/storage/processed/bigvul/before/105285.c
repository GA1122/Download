PassOwnPtr<DOMPatchSupport::Digest> DOMPatchSupport::createDigest(Node* node, UnusedNodesMap* unusedNodesMap)
{
    Digest* digest = new Digest(node);

    SHA1 sha1;

    Node::NodeType nodeType = node->nodeType();
    sha1.addBytes(reinterpret_cast<const uint8_t*>(&nodeType), sizeof(nodeType));
    addStringToSHA1(sha1, node->nodeName());
    addStringToSHA1(sha1, node->nodeValue());

    if (node->nodeType() == Node::ELEMENT_NODE) {
        Node* child = node->firstChild();
        while (child) {
            OwnPtr<Digest> childInfo = createDigest(child, unusedNodesMap);
            addStringToSHA1(sha1, childInfo->m_sha1);
            child = child->nextSibling();
            digest->m_children.append(childInfo.release());
        }
        Element* element = static_cast<Element*>(node);

        if (element->hasAttributesWithoutUpdate()) {
            size_t numAttrs = element->attributeCount();
            SHA1 attrsSHA1;
            for (size_t i = 0; i < numAttrs; ++i) {
                const Attribute* attribute = element->attributeItem(i);
                addStringToSHA1(attrsSHA1, attribute->name().toString());
                addStringToSHA1(attrsSHA1, attribute->value());
            }
            Vector<uint8_t, 20> attrsHash;
            attrsSHA1.computeHash(attrsHash);
            digest->m_attrsSHA1 = base64Encode(reinterpret_cast<const char*>(attrsHash.data()), 10);
            addStringToSHA1(sha1, digest->m_attrsSHA1);
        }
    }

    Vector<uint8_t, 20> hash;
    sha1.computeHash(hash);
    digest->m_sha1 = base64Encode(reinterpret_cast<const char*>(hash.data()), 10);
    if (unusedNodesMap)
        unusedNodesMap->add(digest->m_sha1, digest);
    return adoptPtr(digest);
}
