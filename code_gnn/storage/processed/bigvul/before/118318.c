void AutofillDialogViews::UpdateAccountChooser() {
  account_chooser_->Update();

  bool show_loading = delegate_->ShouldShowSpinner();
  if (show_loading != loading_shield_->visible()) {
    if (show_loading) {
      loading_shield_height_ = std::max(kInitialLoadingShieldHeight,
                                        GetContentsBounds().height());
      ShowDialogInMode(LOADING);
    } else {
      bool show_sign_in = delegate_->ShouldShowSignInWebView();
      ShowDialogInMode(show_sign_in ? SIGN_IN : DETAIL_INPUT);
    }

    InvalidateLayout();
    ContentsPreferredSizeChanged();
  }

  if (footnote_view_) {
    const base::string16 text = delegate_->LegalDocumentsText();
    legal_document_view_->SetText(text);

    if (!text.empty()) {
      const std::vector<gfx::Range>& link_ranges =
          delegate_->LegalDocumentLinks();
      for (size_t i = 0; i < link_ranges.size(); ++i) {
        views::StyledLabel::RangeStyleInfo link_range_info =
            views::StyledLabel::RangeStyleInfo::CreateForLink();
        link_range_info.disable_line_wrapping = false;
        legal_document_view_->AddStyleRange(link_ranges[i], link_range_info);
      }
    }

    footnote_view_->SetVisible(!text.empty());
    ContentsPreferredSizeChanged();
  }

  if (GetWidget())
    GetWidget()->UpdateWindowTitle();
}
