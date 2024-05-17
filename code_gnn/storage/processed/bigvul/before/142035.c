void TextTrackLoader::getNewCues(HeapVector<Member<TextTrackCue>>& outputCues)
{
    DCHECK(m_cueParser);
    if (m_cueParser)
        m_cueParser->getNewCues(outputCues);
}
