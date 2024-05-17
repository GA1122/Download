DocumentVisibilityObserver::DocumentVisibilityObserver(Document& document)
    : m_document(nullptr)
{
    registerObserver(document);
}
