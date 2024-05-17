void Document::notifySeamlessChildDocumentsOfStylesheetUpdate() const
{
    if (!frame())
        return;

    for (Frame* child = frame()->tree()->firstChild(); child; child = child->tree()->nextSibling()) {
        Document* childDocument = child->document();
        if (childDocument->shouldDisplaySeamlesslyWithParent()) {
            ASSERT(childDocument->seamlessParentIFrame()->document() == this);
            childDocument->seamlessParentUpdatedStylesheets();
        }
    }
}
