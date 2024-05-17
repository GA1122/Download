HTMLBodyElement* Document::firstBodyElement() const
{
    if (!documentElement())
        return 0;

    for (HTMLElement* child = Traversal<HTMLElement>::firstChild(*documentElement()); child; child = Traversal<HTMLElement>::nextSibling(*child)) {
        if (isHTMLBodyElement(*child))
            return toHTMLBodyElement(child);
    }

    return 0;
}
