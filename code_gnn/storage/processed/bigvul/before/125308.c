void DocumentTimeline::wake()
{
    m_timing->serviceOnNextFrame();
}
