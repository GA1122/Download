bool HarfBuzzShaper::createHarfBuzzRuns()
{
    if (m_normalizedBufferLength == 1)
        return createHarfBuzzRunsForSingleCharacter();

    Vector<CandidateRun> candidateRuns;
    if (!collectCandidateRuns(m_normalizedBuffer.get(),
        m_normalizedBufferLength, m_font, &candidateRuns, m_run.normalizeSpace()))
        return false;

    if (!resolveCandidateRuns(candidateRuns))
        return false;

    size_t length = candidateRuns.size();
    for (size_t i = 0; i < length; ) {
        CandidateRun& run = candidateRuns[i];
        CandidateRun lastMatchingRun = run;
        for (i++; i < length; i++) {
            if (candidateRuns[i].script != run.script
                || candidateRuns[i].fontData != run.fontData)
                break;
            lastMatchingRun = candidateRuns[i];
        }
        addHarfBuzzRun(run.start, lastMatchingRun.end, run.fontData, run.script);
    }
    return !m_harfBuzzRuns.isEmpty();
}
