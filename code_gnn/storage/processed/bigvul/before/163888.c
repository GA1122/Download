void DevToolsWindow::ShowCertificateViewer(const std::string& cert_chain) {
  std::unique_ptr<base::Value> value = base::JSONReader::Read(cert_chain);
  if (!value || value->GetType() != base::Value::Type::LIST) {
    NOTREACHED();
    return;
  }

  std::unique_ptr<base::ListValue> list =
      base::ListValue::From(std::move(value));
  std::vector<std::string> decoded;
  for (size_t i = 0; i < list->GetSize(); ++i) {
    base::Value* item;
    if (!list->Get(i, &item) || item->GetType() != base::Value::Type::STRING) {
      NOTREACHED();
      return;
    }
    std::string temp;
    if (!item->GetAsString(&temp)) {
      NOTREACHED();
      return;
    }
    if (!base::Base64Decode(temp, &temp)) {
      NOTREACHED();
      return;
    }
    decoded.push_back(temp);
  }

  std::vector<base::StringPiece> cert_string_piece;
  for (const auto& str : decoded)
    cert_string_piece.push_back(str);
  scoped_refptr<net::X509Certificate> cert =
      net::X509Certificate::CreateFromDERCertChain(cert_string_piece);
  if (!cert) {
    NOTREACHED();
    return;
  }

  WebContents* inspected_contents =
      is_docked_ ? GetInspectedWebContents() : main_web_contents_;
  Browser* browser = NULL;
  int tab = 0;
  if (!FindInspectedBrowserAndTabIndex(inspected_contents, &browser, &tab))
    return;
  gfx::NativeWindow parent = browser->window()->GetNativeWindow();
  ::ShowCertificateViewer(inspected_contents, parent, cert.get());
}
