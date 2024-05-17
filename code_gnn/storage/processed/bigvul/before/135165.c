PassRefPtrWillBeRawPtr<Node> Document::cloneNode(bool deep)
{
    RefPtrWillBeRawPtr<Document> clone = cloneDocumentWithoutChildren();
    clone->cloneDataFromDocument(*this);
    if (deep)
        cloneChildNodes(clone.get());
    return clone.release();
}
