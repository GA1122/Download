scoped_ptr<base::Value> LayerTreeHost::AsValue() const {
  scoped_ptr<base::DictionaryValue> state(new base::DictionaryValue());
  state->Set("proxy", proxy_->AsValue().release());
  return state.PassAs<base::Value>();
}
