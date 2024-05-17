int RenderBlock::lineCount(const RootInlineBox* stopRootInlineBox, bool* found) const
{
    int count = 0;

    if (style()->visibility() == VISIBLE) {
        if (childrenInline())
            for (RootInlineBox* box = firstRootBox(); box; box = box->nextRootBox()) {
                count++;
                if (box == stopRootInlineBox) {
                    if (found)
                        *found = true;
                    break;
                }
            }
        else
            for (RenderObject* obj = firstChild(); obj; obj = obj->nextSibling())
                if (shouldCheckLines(obj)) {
                    bool recursiveFound = false;
                    count += toRenderBlock(obj)->lineCount(stopRootInlineBox, &recursiveFound);
                    if (recursiveFound) {
                        if (found)
                            *found = true;
                        break;
                    }
                }
    }
    return count;
}
