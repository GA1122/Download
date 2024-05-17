bool Document::isLayoutTimerActive()
{
    return view() && view()->layoutPending() && !minimumLayoutDelay();
}
