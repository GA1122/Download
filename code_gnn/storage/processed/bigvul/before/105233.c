void HTMLElement::childrenChanged(bool changedByParser, Node* beforeChange, Node* afterChange, int childCountDelta)
{
    StyledElement::childrenChanged(changedByParser, beforeChange, afterChange, childCountDelta);
    adjustDirectionalityIfNeededAfterChildrenChanged(beforeChange, childCountDelta);
}
