void AwContents::SetExtraHeadersForUrl(JNIEnv* env, jobject obj,
                                       jstring url, jstring jextra_headers) {
  std::string extra_headers;
  if (jextra_headers)
    extra_headers = ConvertJavaStringToUTF8(env, jextra_headers);
  AwResourceContext* resource_context = static_cast<AwResourceContext*>(
      AwBrowserContext::FromWebContents(web_contents_.get())->
      GetResourceContext());
  resource_context->SetExtraHeaders(GURL(ConvertJavaStringToUTF8(env, url)),
                                    extra_headers);
}
