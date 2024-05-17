PassRefPtr<DocumentTimeline> DocumentTimeline::create(Document* document, PassOwnPtr<PlatformTiming> timing)
{
    return adoptRef(new DocumentTimeline(document, timing));
}
