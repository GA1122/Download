PassRefPtrWillBeRawPtr<Range> Document::createRange()
{
    return Range::create(*this);
}
