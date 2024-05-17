TouchList* Document::createTouchList(HeapVector<Member<Touch>>& touches) const
{
    return TouchList::adopt(touches);
}
