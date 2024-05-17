void SVGDocumentExtensions::serviceAnimations(double monotonicAnimationStartTime)
{
    WillBeHeapVector<RefPtrWillBeMember<SVGSVGElement> > timeContainers;
    timeContainers.appendRange(m_timeContainers.begin(), m_timeContainers.end());
    WillBeHeapVector<RefPtrWillBeMember<SVGSVGElement> >::iterator end = timeContainers.end();
    for (WillBeHeapVector<RefPtrWillBeMember<SVGSVGElement> >::iterator itr = timeContainers.begin(); itr != end; ++itr)
        (*itr)->timeContainer()->serviceAnimations(monotonicAnimationStartTime);
}
