void TextTrackLoader::getNewRegions(HeapVector<Member<VTTRegion>>& outputRegions)
{
    DCHECK(m_cueParser);
    if (m_cueParser)
        m_cueParser->getNewRegions(outputRegions);
}
