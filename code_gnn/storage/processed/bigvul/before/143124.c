AnimationClock& Document::animationClock()
{
    DCHECK(page());
    return page()->animator().clock();
}
