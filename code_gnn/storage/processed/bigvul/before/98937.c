void HTMLConstructionSite::insertTextNode(const String& characters)
{
    AttachmentSite site;
    site.parent = currentElement();
    site.nextChild = 0;
    if (shouldFosterParent())
        findFosterSite(site);

    Node* previousChild = site.nextChild ? site.nextChild->previousSibling() : site.parent->lastChild();
    if (previousChild && previousChild->isTextNode()) {
        CharacterData* textNode = static_cast<CharacterData*>(previousChild);
        textNode->parserAppendData(characters);
        return;
    }

    attachAtSite(site, Text::create(m_document, characters));
}
