void Document::loadEventDelayTimerFired(Timer<Document>*)
{
    if (frame())
        frame()->loader().checkCompleted();
}
