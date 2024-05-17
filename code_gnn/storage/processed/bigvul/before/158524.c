HeapVector<ScriptSourceCode> CreateSourcesVector(
    const WebScriptSource* sources_in,
    unsigned num_sources) {
  HeapVector<ScriptSourceCode> sources;
  sources.Append(sources_in, num_sources);
  return sources;
}
