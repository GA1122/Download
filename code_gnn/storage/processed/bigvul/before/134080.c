 TextInputType InputMethodBase::GetTextInputType() const {
   TextInputClient* client = GetTextInputClient();
   return client ? client->GetTextInputType() : TEXT_INPUT_TYPE_NONE;
}
