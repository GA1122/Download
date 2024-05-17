LinkChangeSerializerMarkupAccumulator::LinkChangeSerializerMarkupAccumulator(PageSerializer* serializer, const Document& document, WillBeHeapVector<RawPtrWillBeMember<Node>>& nodes, HashMap<String, String>& rewriteURLs, const String& rewriteFolder)
    : SerializerMarkupAccumulator(serializer, document, nodes)
    , m_rewriteURLs(rewriteURLs)
    , m_rewriteFolder(rewriteFolder)
{
}
