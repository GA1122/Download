HTMLLinkElement* Document::linkManifest() const
{
    HTMLHeadElement* head = this->head();
    if (!head)
        return 0;

    for (HTMLLinkElement* linkElement = Traversal<HTMLLinkElement>::firstChild(*head); linkElement; linkElement = Traversal<HTMLLinkElement>::nextSibling(*linkElement)) {
        if (!linkElement->relAttribute().isManifest())
            continue;
        return linkElement;
    }

    return 0;
}
