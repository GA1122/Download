void CSSStyleSheetResource::NotifyFinished() {
  if (Data())
    SetDecodedSheetText(DecodedText());

  Resource::NotifyFinished();

  ClearData();
}
