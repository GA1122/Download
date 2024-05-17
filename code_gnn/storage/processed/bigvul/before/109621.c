PassRefPtr<TouchList> Document::createTouchList(Vector<RefPtr<Touch> >& touches) const
{
    return TouchList::create(touches);
}
