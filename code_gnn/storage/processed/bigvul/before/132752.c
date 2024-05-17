void ChromotingInstance::SetCapabilities(const std::string& capabilities) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetString("capabilities", capabilities);
  PostLegacyJsonMessage("setCapabilities", data.Pass());
}
