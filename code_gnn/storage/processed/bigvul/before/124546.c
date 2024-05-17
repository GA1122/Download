RenderBlock* RenderBlock::firstLineBlock() const
{
    RenderBlock* firstLineBlock = const_cast<RenderBlock*>(this);
    bool hasPseudo = false;
    while (true) {
        hasPseudo = firstLineBlock->style()->hasPseudoStyle(FIRST_LINE);
        if (hasPseudo)
            break;
        RenderObject* parentBlock = firstLineBlock->parent();
        if (firstLineBlock->isReplaced() || firstLineBlock->isFloating()
            || !parentBlock || parentBlock->firstChild() != firstLineBlock
            || (!parentBlock->isRenderBlockFlow() && !parentBlock->isRenderButton()))
            break;
        ASSERT_WITH_SECURITY_IMPLICATION(parentBlock->isRenderBlock());
        firstLineBlock = toRenderBlock(parentBlock);
    }

    if (!hasPseudo)
        return 0;

    return firstLineBlock;
}
