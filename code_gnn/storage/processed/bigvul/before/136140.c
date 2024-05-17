HTMLElement* Document::body() const
{
    if (!documentElement())
        return 0;

    for (HTMLElement* child = Traversal<HTMLElement>::firstChild(*documentElement()); child; child = Traversal<HTMLElement>::nextSibling(*child)) {
        if (isHTMLFrameSetElement(*child) || isHTMLBodyElement(*child))
            return child;
    }

    return 0;
}
