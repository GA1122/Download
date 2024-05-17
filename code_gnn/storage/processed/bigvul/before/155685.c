void AuthenticatorSelectAccountSheetModel::SetCurrentSelection(int selected) {
  DCHECK_LE(0, selected);
  DCHECK_LT(static_cast<size_t>(selected), dialog_model()->responses().size());
  selected_ = selected;
}
