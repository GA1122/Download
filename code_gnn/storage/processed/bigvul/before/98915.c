void HTMLConstructionSite::findFosterSite(AttachmentSite& site)
{
    HTMLElementStack::ElementRecord* lastTableElementRecord = m_openElements.topmost(tableTag.localName());
    if (lastTableElementRecord) {
        Element* lastTableElement = lastTableElementRecord->element();
        if (Node* parent = lastTableElement->parent()) {
            site.parent = parent;
            site.nextChild = lastTableElement;
            return;
        }
        site.parent = lastTableElementRecord->next()->element();
        site.nextChild = 0;
        return;
    }
    site.parent = m_openElements.bottom();  
    site.nextChild = 0;
}
