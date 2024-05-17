void Document::setRootScroller(Element* newScroller, ExceptionState& exceptionState)
{
    m_rootScrollerController->set(newScroller);
}
