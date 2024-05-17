void FrameFetchContext::DispatchDidChangeResourcePriority(
    unsigned long identifier,
    ResourceLoadPriority load_priority,
    int intra_priority_value) {
  if (IsDetached())
    return;
  TRACE_EVENT1("devtools.timeline", "ResourceChangePriority", "data",
               InspectorChangeResourcePriorityEvent::Data(
                   MasterDocumentLoader(), identifier, load_priority));
  probe::didChangeResourcePriority(GetFrame(), MasterDocumentLoader(),
                                   identifier, load_priority);
}
