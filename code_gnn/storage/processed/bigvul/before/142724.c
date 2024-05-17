PassOwnPtr<TracedValue> FrameLoader::toTracedValue() const
{
    OwnPtr<TracedValue> tracedValue = TracedValue::create();
    tracedValue->beginDictionary("frame");
    tracedValue->setString("id_ref", String::format("0x%" PRIx64, static_cast<uint64_t>(reinterpret_cast<uintptr_t>(m_frame.get()))));
    tracedValue->endDictionary();
    tracedValue->setBoolean("isLoadingMainFrame", isLoadingMainFrame());
    tracedValue->setString("stateMachine", m_stateMachine.toString());
    tracedValue->setString("provisionalDocumentLoaderURL", m_provisionalDocumentLoader ? m_provisionalDocumentLoader->url() : String());
    tracedValue->setString("documentLoaderURL", m_documentLoader ? m_documentLoader->url() : String());
    return tracedValue;
}
