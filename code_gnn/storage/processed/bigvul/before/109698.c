HTMLHeadElement* Document::head()
{
    Node* de = documentElement();
    if (!de)
        return 0;

    for (Node* node = de->firstChild(); node; node = node->nextSibling()) {
        if (node->hasTagName(headTag))
            return toHTMLHeadElement(node);
    }
    return 0;
}
