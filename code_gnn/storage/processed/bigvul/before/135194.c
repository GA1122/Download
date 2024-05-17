PassRefPtrWillBeRawPtr<TouchList> Document::createTouchList(WillBeHeapVector<RefPtrWillBeMember<Touch>>& touches) const
{
    return TouchList::adopt(touches);
}
