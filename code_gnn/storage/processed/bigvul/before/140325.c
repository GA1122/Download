static EditorClient& emptyEditorClient() {
  DEFINE_STATIC_LOCAL(EmptyEditorClient, client, ());
  return client;
}
