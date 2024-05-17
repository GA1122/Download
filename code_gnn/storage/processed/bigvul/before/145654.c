void ExtensionOptionsGuest::DidInitialize(
    const base::DictionaryValue& create_params) {
  ExtensionsAPIClient::Get()->AttachWebContentsHelpers(web_contents());
  web_contents()->GetController().LoadURL(options_page_,
                                          content::Referrer(),
                                          ui::PAGE_TRANSITION_LINK,
                                          std::string());
}
