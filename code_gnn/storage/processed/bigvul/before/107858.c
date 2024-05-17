BlockedPlugin::BlockedPlugin(RenderView* render_view,
                             WebFrame* frame,
                             const webkit::npapi::PluginGroup& info,
                             const WebPluginParams& params,
                             const WebPreferences& preferences,
                             int template_id,
                             const string16& message,
                             bool is_blocked_for_prerendering)
    : RenderViewObserver(render_view),
      frame_(frame),
      plugin_params_(params),
      is_blocked_for_prerendering_(is_blocked_for_prerendering),
      hidden_(false) {
  const base::StringPiece template_html(
      ResourceBundle::GetSharedInstance().GetRawDataResource(template_id));

  DCHECK(!template_html.empty()) << "unable to load template. ID: "
                                 << template_id;

  DictionaryValue values;
  values.SetString("message", message);
  name_ = info.GetGroupName();
  values.SetString("name", name_);
  values.SetString("hide", l10n_util::GetStringUTF8(IDS_PLUGIN_HIDE));

  std::string html_data = jstemplate_builder::GetTemplatesHtml(
      template_html, &values, "t");

  plugin_ = webkit::npapi::WebViewPlugin::Create(this,
                                                 preferences,
                                                 html_data,
                                                 GURL(kBlockedPluginDataURL));
}
