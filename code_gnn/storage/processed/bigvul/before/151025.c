ResponseWriter::ResponseWriter(base::WeakPtr<DevToolsUIBindings> bindings,
                               int stream_id)
    : bindings_(bindings),
      stream_id_(stream_id) {
}
