Node* HTMLElement::insertAdjacent(const String& where, Node* newChild, ExceptionCode& ec)
{

    if (equalIgnoringCase(where, "beforeBegin")) {
        ContainerNode* parent = this->parentNode();
        return (parent && parent->insertBefore(newChild, this, ec)) ? newChild : 0;
    }

    if (equalIgnoringCase(where, "afterBegin"))
        return insertBefore(newChild, firstChild(), ec) ? newChild : 0;

    if (equalIgnoringCase(where, "beforeEnd"))
        return appendChild(newChild, ec) ? newChild : 0;

    if (equalIgnoringCase(where, "afterEnd")) {
        ContainerNode* parent = this->parentNode();
        return (parent && parent->insertBefore(newChild, nextSibling(), ec)) ? newChild : 0;
    }
    
    ec = NOT_SUPPORTED_ERR;
    return 0;
}
