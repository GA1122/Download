WebPluginImpl::WebPluginImpl(
    WebFrame* webframe, const WebPluginParams& params,
    const base::WeakPtr<WebPluginPageDelegate>& page_delegate)
    : windowless_(false),
      window_(NULL),
      accepts_input_events_(false),
      page_delegate_(page_delegate),
      webframe_(webframe),
      delegate_(NULL),
      container_(NULL),
      plugin_url_(params.url),
      load_manually_(params.loadManually),
      first_geometry_update_(true),
      ignore_response_error_(false),
      mime_type_(params.mimeType.utf8()),
      ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)) {
  DCHECK_EQ(params.attributeNames.size(), params.attributeValues.size());
  StringToLowerASCII(&mime_type_);

  for (size_t i = 0; i < params.attributeNames.size(); ++i) {
    arg_names_.push_back(params.attributeNames[i].utf8());
    arg_values_.push_back(params.attributeValues[i].utf8());
  }
}
