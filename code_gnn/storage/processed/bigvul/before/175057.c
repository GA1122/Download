Sample::Sample(int sampleID, const char* url)
{
    init();
    mSampleID = sampleID;
    mUrl = strdup(url);
    ALOGV("create sampleID=%d, url=%s", mSampleID, mUrl);
}
