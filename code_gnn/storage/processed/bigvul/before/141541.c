ChooserContextBase::Object::Object(GURL requesting_origin,
                                   GURL embedding_origin,
                                   base::DictionaryValue* value,
                                   content_settings::SettingSource source,
                                   bool incognito)
    : requesting_origin(requesting_origin),
      embedding_origin(embedding_origin),
      source(source),
      incognito(incognito) {
  this->value.Swap(value);
}
