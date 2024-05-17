void Document::fullScreenChangeDelayTimerFired(Timer<Document>*)
{
    RefPtr<Document> protectDocument(this);
    Deque<RefPtr<Node> > changeQueue;
    m_fullScreenChangeEventTargetQueue.swap(changeQueue);
    Deque<RefPtr<Node> > errorQueue;
    m_fullScreenErrorEventTargetQueue.swap(errorQueue);

    while (!changeQueue.isEmpty()) {
        RefPtr<Node> node = changeQueue.takeFirst();
        if (!node)
            node = documentElement();
        if (!node)
            continue;

        if (!contains(node.get()) && !node->inDocument())
            changeQueue.append(documentElement());
        
        node->dispatchEvent(Event::create(eventNames().webkitfullscreenchangeEvent, true, false));
    }

    while (!errorQueue.isEmpty()) {
        RefPtr<Node> node = errorQueue.takeFirst();
        if (!node)
            node = documentElement();
        if (!node)
            continue;
        
        if (!contains(node.get()) && !node->inDocument())
            errorQueue.append(documentElement());
        
        node->dispatchEvent(Event::create(eventNames().webkitfullscreenerrorEvent, true, false));
    }
}
