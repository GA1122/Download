static inline LayoutObject* endOfContinuations(LayoutObject* layoutObject)
{
    LayoutObject* prev = nullptr;
    LayoutObject* cur = layoutObject;

    if (!cur->isLayoutInline() && !cur->isLayoutBlock())
        return nullptr;

    while (cur) {
        prev = cur;
        if (cur->isLayoutInline())
            cur = toLayoutInline(cur)->continuation();
        else
            cur = toLayoutBlock(cur)->continuation();
    }

    return prev;
}
