static WebKitWebDataSource* webkit_web_frame_get_data_source_from_core_loader(WebCore::DocumentLoader* loader)
{
    return loader ? static_cast<WebKit::DocumentLoader*>(loader)->dataSource() : NULL;
}
