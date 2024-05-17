static void webkit_web_frame_load_data(WebKitWebFrame* frame, const gchar* content, const gchar* mimeType, const gchar* encoding, const gchar* baseURL, const gchar* unreachableURL)
{
    Frame* coreFrame = core(frame);
    ASSERT(coreFrame);

    KURL baseKURL = baseURL ? KURL(KURL(), String::fromUTF8(baseURL)) : blankURL();

    ResourceRequest request(baseKURL);

    RefPtr<SharedBuffer> sharedBuffer = SharedBuffer::create(content, strlen(content));
    SubstituteData substituteData(sharedBuffer.release(),
                                  mimeType ? String::fromUTF8(mimeType) : String::fromUTF8("text/html"),
                                  encoding ? String::fromUTF8(encoding) : String::fromUTF8("UTF-8"),
                                  KURL(KURL(), String::fromUTF8(unreachableURL)),
                                  KURL(KURL(), String::fromUTF8(unreachableURL)));

    coreFrame->loader()->load(request, substituteData, false);
}
