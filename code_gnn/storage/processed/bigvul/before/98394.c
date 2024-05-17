WebKitWebDataSource* webkit_web_frame_get_provisional_data_source(WebKitWebFrame* frame)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_FRAME(frame), NULL);

    Frame* coreFrame = core(frame);
    return webkit_web_frame_get_data_source_from_core_loader(coreFrame->loader()->provisionalDocumentLoader());
}
