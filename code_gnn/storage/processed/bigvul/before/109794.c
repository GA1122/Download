void Document::seamlessParentUpdatedStylesheets()
{
    m_styleEngine->didModifySeamlessParentStyleSheet();
    styleResolverChanged(RecalcStyleImmediately);
}
