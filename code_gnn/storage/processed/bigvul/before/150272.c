void AnnotateFieldsWithSignatures(
    std::vector<blink::WebFormControlElement>* fields,
    const blink::WebString& form_signature) {
  for (blink::WebFormControlElement& control_element : *fields) {
    FieldSignature field_signature = CalculateFieldSignatureByNameAndType(
        control_element.NameForAutofill().Utf16(),
        control_element.FormControlTypeForAutofill().Utf8());
    control_element.SetAttribute(
        WebString::FromASCII(kDebugAttributeForFieldSignature),
        WebString::FromUTF8(base::NumberToString(field_signature)));
    control_element.SetAttribute(
        blink::WebString::FromASCII(kDebugAttributeForFormSignature),
        form_signature);
  }
}
