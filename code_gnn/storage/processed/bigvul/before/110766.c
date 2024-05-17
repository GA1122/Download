AutocompleteController::~AutocompleteController() {
  result_.Reset();   
  Stop(false);

  for (ACProviders::iterator i(providers_.begin()); i != providers_.end(); ++i)
    (*i)->Release();

  providers_.clear();   
}
