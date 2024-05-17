SerializerMarkupAccumulator::SerializerMarkupAccumulator(PageSerializer* serializer, Document* document, Vector<Node*>* nodes)
    : MarkupAccumulator(nodes, ResolveAllURLs)
    , m_serializer(serializer)
    , m_document(document)
{
}
