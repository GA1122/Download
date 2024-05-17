void Document::setDesignMode(InheritedBool value)
{
    m_designMode = value;
    for (Frame* frame = m_frame; frame && frame->document(); frame = frame->tree()->traverseNext(m_frame))
        frame->document()->scheduleForcedStyleRecalc();
}
