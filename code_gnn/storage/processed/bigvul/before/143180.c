HeapVector<Member<Element>> Document::elementsFromPoint(int x, int y) const
{
    if (!layoutView())
        return HeapVector<Member<Element>>();
    return TreeScope::elementsFromPoint(x, y);
}
