RootInlineBox* RenderBlock::lineAtIndex(int i) const
{
    ASSERT(i >= 0);

    if (style()->visibility() != VISIBLE)
        return 0;

    if (childrenInline()) {
        for (RootInlineBox* box = firstRootBox(); box; box = box->nextRootBox())
            if (!i--)
                return box;
    } else {
        for (RenderObject* child = firstChild(); child; child = child->nextSibling()) {
            if (!shouldCheckLines(child))
                continue;
            if (RootInlineBox* box = toRenderBlock(child)->lineAtIndex(i))
                return box;
        }
    }

    return 0;
}
