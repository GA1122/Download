unsigned long long Timing::responseEnd() const
{
    DocumentLoadTiming* timing = documentLoadTiming();
    if (!timing)
        return 0;

    return toIntegerMilliseconds(timing->responseEnd);
}
