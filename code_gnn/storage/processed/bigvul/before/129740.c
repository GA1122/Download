void ResourceFetcher::resourceTimingReportTimerFired(Timer<ResourceFetcher>* timer)
{
    ASSERT_UNUSED(timer, timer == &m_resourceTimingReportTimer);
    HashMap<RefPtr<ResourceTimingInfo>, bool> timingReports;
    timingReports.swap(m_scheduledResourceTimingReports);
    HashMap<RefPtr<ResourceTimingInfo>, bool>::iterator end = timingReports.end();
    for (HashMap<RefPtr<ResourceTimingInfo>, bool>::iterator it = timingReports.begin(); it != end; ++it) {
        RefPtr<ResourceTimingInfo> info = it->key;
        bool isMainResource = it->value;
        reportResourceTiming(info.get(), document(), isMainResource);
    }
}
