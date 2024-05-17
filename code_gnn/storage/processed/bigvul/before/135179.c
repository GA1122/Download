PassRefPtrWillBeRawPtr<DocumentFragment> Document::createDocumentFragment()
{
    return DocumentFragment::create(*this);
}
