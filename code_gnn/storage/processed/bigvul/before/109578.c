HTMLElement* Document::body() const
{
    if (!documentElement())
        return 0;

    for (Node* child = documentElement()->firstChild(); child; child = child->nextSibling()) {
        if (child->hasTagName(framesetTag) || child->hasTagName(bodyTag))
            return toHTMLElement(child);
    }

    return 0;
}
