AutocompleteProvider::AutocompleteProvider(ACProviderListener* listener,
                                           Profile* profile,
                                           const char* name)
    : profile_(profile),
      listener_(listener),
      done_(true),
      name_(name) {
}
