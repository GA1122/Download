static inline bool matchesAdjacentRun(UScriptCode* scriptExtensions, int length,
    CandidateRun& adjacentRun)
{
    for (int i = 0; i < length; i++) {
        if (scriptExtensions[i] == adjacentRun.script)
            return true;
    }
    return false;
}
