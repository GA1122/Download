void HTMLInputElement::EndColorChooser() {
  if (ColorChooserClient* client = input_type_->GetColorChooserClient())
    client->DidEndChooser();
}
