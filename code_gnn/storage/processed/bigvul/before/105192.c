static void completeURLs(Node* node, const String& baseURL)
{
    Vector<AttributeChange> changes;

    KURL parsedBaseURL(ParsedURLString, baseURL);

    Node* end = node->traverseNextSibling();
    for (Node* n = node; n != end; n = n->traverseNextNode()) {
        if (n->isElementNode()) {
            Element* e = static_cast<Element*>(n);
            if (!e->hasAttributes())
                continue;
            unsigned length = e->attributeCount();
            for (unsigned i = 0; i < length; i++) {
                Attribute* attribute = e->attributeItem(i);
                if (e->isURLAttribute(*attribute))
                    changes.append(AttributeChange(e, attribute->name(), KURL(parsedBaseURL, attribute->value()).string()));
            }
        }
    }

    size_t numChanges = changes.size();
    for (size_t i = 0; i < numChanges; ++i)
        changes[i].apply();
}
