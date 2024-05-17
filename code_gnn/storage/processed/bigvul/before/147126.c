unsigned long DocumentLoader::MainResourceIdentifier() const {
  return main_resource_ ? main_resource_->Identifier() : 0;
}
