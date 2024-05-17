void Document::addDocumentToFullScreenChangeEventQueue(Document* doc)
{
    ASSERT(doc);
    Node* target = doc->webkitFullscreenElement();
    if (!target)
        target = doc->webkitCurrentFullScreenElement();
    if (!target)
        target = doc;
    m_fullScreenChangeEventTargetQueue.append(target);
}
