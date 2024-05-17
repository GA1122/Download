    OverflowEventDispatcher(const RenderBlock* block)
        : m_block(block)
        , m_hadHorizontalLayoutOverflow(false)
        , m_hadVerticalLayoutOverflow(false)
    {
        m_shouldDispatchEvent = !m_block->isAnonymous() && m_block->hasOverflowClip() && m_block->document().hasListenerType(Document::OVERFLOWCHANGED_LISTENER);
        if (m_shouldDispatchEvent) {
            m_hadHorizontalLayoutOverflow = m_block->hasHorizontalLayoutOverflow();
            m_hadVerticalLayoutOverflow = m_block->hasVerticalLayoutOverflow();
        }
    }
