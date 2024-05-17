views::View* PageInfoWindowView::GetExtraView() {
  if (!cert_id_)
    return NULL;
  scoped_refptr<net::X509Certificate> cert;
  CertStore::GetSharedInstance()->RetrieveCert(cert_id_, &cert);
  if (!cert.get() || !cert->os_cert_handle())
    return NULL;

  views::View* button_container = new views::View();
  views::GridLayout* layout = new views::GridLayout(button_container);
  button_container->SetLayoutManager(layout);

  views::ColumnSet* column_set = layout->AddColumnSet(0);
  column_set->AddPaddingColumn(0, kHorizontalPadding);
  column_set->AddColumn(views::GridLayout::LEADING,
                        views::GridLayout::LEADING, 0,
                        views::GridLayout::USE_PREF, 0, 0);
  layout->StartRow(0, 0);
  layout->AddView(new views::NativeButton(this,
      l10n_util::GetString(IDS_PAGEINFO_CERT_INFO_BUTTON)));

  return button_container;
}
