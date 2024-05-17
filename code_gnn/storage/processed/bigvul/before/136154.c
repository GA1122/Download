Vector<Element*> Document::elementsFromPoint(int x, int y) const
{
    if (!layoutView())
        return Vector<Element*>();
    return TreeScope::elementsFromPoint(x, y);
}
