void RenderBlock::clearTruncation()
{
    if (style()->visibility() == VISIBLE) {
        if (childrenInline() && hasMarkupTruncation()) {
            setHasMarkupTruncation(false);
            for (RootInlineBox* box = firstRootBox(); box; box = box->nextRootBox())
                box->clearTruncation();
        } else {
            for (RenderObject* obj = firstChild(); obj; obj = obj->nextSibling()) {
                if (shouldCheckLines(obj))
                    toRenderBlock(obj)->clearTruncation();
            }
        }
    }
}
