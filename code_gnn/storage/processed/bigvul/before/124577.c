void RenderBlock::layout()
{
    OverflowEventDispatcher dispatcher(this);

    updateFirstLetter();

    layoutBlock(false);

    if (hasControlClip() && m_overflow)
        clearLayoutOverflow();

    invalidateBackgroundObscurationStatus();
}
