AnimationClock& Document::animationClock()
{
    ASSERT(page());
    return page()->animator().clock();
}
