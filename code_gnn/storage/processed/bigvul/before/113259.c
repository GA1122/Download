AutocompleteController::AutocompleteController(
    Profile* profile,
    AutocompleteControllerDelegate* delegate)
    : delegate_(delegate),
      keyword_provider_(NULL),
      done_(true),
      in_start_(false),
      profile_(profile) {
  search_provider_ = new SearchProvider(this, profile);
  providers_.push_back(search_provider_);
  bool hqp_enabled = !CommandLine::ForCurrentProcess()->HasSwitch(
                         switches::kDisableHistoryQuickProvider);
  if (hqp_enabled)
    providers_.push_back(new HistoryQuickProvider(this, profile));
#if !defined(OS_ANDROID)
  keyword_provider_ = new KeywordProvider(this, profile);
  providers_.push_back(keyword_provider_);
#endif   
  providers_.push_back(new HistoryURLProvider(this, profile));
  providers_.push_back(new ShortcutsProvider(this, profile));
  providers_.push_back(new HistoryContentsProvider(this, profile, hqp_enabled));
  providers_.push_back(new BuiltinProvider(this, profile));
  providers_.push_back(new ExtensionAppProvider(this, profile));
  for (ACProviders::iterator i(providers_.begin()); i != providers_.end(); ++i)
    (*i)->AddRef();
}
