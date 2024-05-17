static inline void resolveRunBasedOnScriptValue(Vector<CandidateRun>& runs,
    CandidateRun& run, size_t i, size_t length, size_t& nextResolvedRun)
{
    if (run.script != USCRIPT_COMMON)
        return;

    if (i > 0 && runs[i - 1].script != USCRIPT_COMMON) {
        run.script = runs[i - 1].script;
        return;
    }

    for (size_t j = i + 1; j < length; j++) {
        if (runs[j].script != USCRIPT_COMMON
            && runs[j].script != USCRIPT_INHERITED) {
            nextResolvedRun = j;
            break;
        }
    }
}
