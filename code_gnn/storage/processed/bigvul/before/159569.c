String GetTypeExtension(Document* document,
                        const StringOrDictionary& string_or_options,
                        ExceptionState& exception_state) {
  if (string_or_options.IsNull())
    return g_empty_string;

  if (string_or_options.IsString()) {
    UseCounter::Count(document,
                      WebFeature::kDocumentCreateElement2ndArgStringHandling);
    return string_or_options.GetAsString();
  }

  if (string_or_options.IsDictionary()) {
    Dictionary dict = string_or_options.GetAsDictionary();
    ElementCreationOptions impl;
    V8ElementCreationOptions::ToImpl(dict.GetIsolate(), dict.V8Value(), impl,
                                     exception_state);
    if (exception_state.HadException())
      return g_empty_string;

    if (impl.hasIs())
      return impl.is();
  }

  return g_empty_string;
}
