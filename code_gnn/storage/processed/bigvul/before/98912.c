PassRefPtr<Element> HTMLConstructionSite::createHTMLElementFromElementRecord(HTMLElementStack::ElementRecord* record)
{
    return createHTMLElementFromSavedElement(record->element());
}
