TouchList* Document::createTouchList(HeapVector<Member<Touch>>& touches) const {
  return TouchList::Adopt(touches);
}
