BrowserContextKeyedAPIFactory<WebstoreAPI>* WebstoreAPI::GetFactoryInstance() {
  return g_factory.Pointer();
}
