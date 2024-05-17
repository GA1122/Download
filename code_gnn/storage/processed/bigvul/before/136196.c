SerializerMarkupAccumulator::SerializerMarkupAccumulator(PageSerializer* serializer, const Document& document, WillBeHeapVector<RawPtrWillBeMember<Node>>& nodes)
    : MarkupAccumulator(ResolveAllURLs)
    , m_serializer(serializer)
    , m_document(&document)
    , m_nodes(nodes)
{
}
