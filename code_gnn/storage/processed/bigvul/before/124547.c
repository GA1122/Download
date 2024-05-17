int RenderBlock::firstLineBoxBaseline() const
{
    if (isWritingModeRoot() && !isRubyRun())
        return -1;

    if (childrenInline()) {
        if (firstLineBox())
            return firstLineBox()->logicalTop() + style(true)->fontMetrics().ascent(firstRootBox()->baselineType());
        else
            return -1;
    }
    else {
        for (RenderBox* curr = firstChildBox(); curr; curr = curr->nextSiblingBox()) {
            if (!curr->isFloatingOrOutOfFlowPositioned()) {
                int result = curr->firstLineBoxBaseline();
                if (result != -1)
                    return curr->logicalTop() + result;  
            }
        }
    }

    return -1;
}
