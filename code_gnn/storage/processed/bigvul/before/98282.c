WTF::PassRefPtr<WebCore::DocumentLoader> FrameLoaderClient::createDocumentLoader(const WebCore::ResourceRequest& request, const SubstituteData& substituteData)
{
    RefPtr<WebKit::DocumentLoader> loader = WebKit::DocumentLoader::create(request, substituteData);

    WebKitWebDataSource* webDataSource = webkit_web_data_source_new_with_loader(loader.get());
    loader->setDataSource(webDataSource);
    g_object_unref(webDataSource);

    return loader.release();
}
