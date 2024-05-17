HTMLLinkElement* Document::linkDefaultPresentation() const
{
    HTMLHeadElement* head = this->head();
    if (!head)
        return nullptr;

    for (HTMLLinkElement* linkElement = Traversal<HTMLLinkElement>::firstChild(*head); linkElement; linkElement = Traversal<HTMLLinkElement>::nextSibling(*linkElement)) {
        if (!linkElement->relAttribute().isDefaultPresentation())
            continue;
        return linkElement;
    }

    return nullptr;
}
