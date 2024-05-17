void DocumentTimeline::DocumentTimelineTiming::serviceOnNextFrame()
{
    if (m_timeline->m_document->view())
        m_timeline->m_document->view()->scheduleAnimation();
}
