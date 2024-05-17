void FrameView::performLayout(RenderObject* rootForThisLayout, bool inSubtreeLayout)
{
    TRACE_EVENT0("blink", "FrameView::performLayout");

    ScriptForbiddenScope forbidScript;

    ASSERT(!isInPerformLayout());
    lifecycle().advanceTo(DocumentLifecycle::InPerformLayout);

    TemporaryChange<bool> changeInPerformLayout(m_inPerformLayout, true);


    LayoutState layoutState(*rootForThisLayout);

    forceLayoutParentViewIfNeeded();

    rootForThisLayout->layout();
    gatherDebugLayoutRects(rootForThisLayout);

    ResourceLoadPriorityOptimizer::resourceLoadPriorityOptimizer()->updateAllImageResourcePriorities();

    lifecycle().advanceTo(DocumentLifecycle::AfterPerformLayout);
}
