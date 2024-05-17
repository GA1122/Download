static inline void resolveRunBasedOnScriptExtensions(Vector<CandidateRun>& runs,
    CandidateRun& run, size_t i, size_t length, UScriptCode* scriptExtensions,
    int extensionsLength, size_t& nextResolvedRun)
{
    if (extensionsLength <= 1)
        return;

    if (i > 0 && matchesAdjacentRun(scriptExtensions, extensionsLength, runs[i - 1])) {
        run.script = runs[i - 1].script;
        return;
    }

    for (size_t j = i + 1; j < length; j++) {
        if (runs[j].script != USCRIPT_COMMON
            && runs[j].script != USCRIPT_INHERITED
            && matchesAdjacentRun(scriptExtensions, extensionsLength, runs[j])) {
            nextResolvedRun = j;
            break;
        }
    }
}
