void PageInfoWindowView::ButtonPressed(
    views::Button* sender, const views::Event& event) {
  DCHECK(cert_id_ != 0);
  ShowCertDialog(cert_id_);
}
