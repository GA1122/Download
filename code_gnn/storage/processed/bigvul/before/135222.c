WillBeHeapVector<RawPtrWillBeMember<Element>> Document::elementsFromPoint(int x, int y) const
{
    if (!layoutView())
        return WillBeHeapVector<RawPtrWillBeMember<Element>>();
    return TreeScope::elementsFromPoint(x, y);
}
