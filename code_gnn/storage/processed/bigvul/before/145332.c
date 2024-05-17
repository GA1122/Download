Node::InsertionNotificationRequest HTMLAnchorElement::insertedInto(ContainerNode* insertionPoint)
{
    InsertionNotificationRequest request = HTMLElement::insertedInto(insertionPoint);
    logAddElementIfIsolatedWorldAndInDocument("a", hrefAttr);
    return request;
}
