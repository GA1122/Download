void AutocompleteEditModel::OnResultChanged(bool default_match_changed) {
  const bool was_open = popup_->IsOpen();
  if (default_match_changed) {
    string16 inline_autocomplete_text;
    string16 keyword;
    bool is_keyword_hint = false;
    const AutocompleteResult& result = this->result();
    const AutocompleteResult::const_iterator match(result.default_match());
    if (match != result.end()) {
      if ((match->inline_autocomplete_offset != string16::npos) &&
          (match->inline_autocomplete_offset <
           match->fill_into_edit.length())) {
        inline_autocomplete_text =
            match->fill_into_edit.substr(match->inline_autocomplete_offset);
      }

      if (!prerender::IsOmniboxEnabled(profile_))
        DoPreconnect(*match);


      match->GetKeywordUIState(profile_, &keyword, &is_keyword_hint);
    }

    popup_->OnResultChanged();
    OnPopupDataChanged(inline_autocomplete_text, NULL, keyword,
                       is_keyword_hint);
  } else {
    popup_->OnResultChanged();
  }

  if (popup_->IsOpen()) {
    PopupBoundsChangedTo(popup_->view()->GetTargetBounds());
  } else if (was_open) {
    InternalSetUserText(UserTextFromDisplayText(view_->GetText()));
    has_temporary_text_ = false;
    PopupBoundsChangedTo(gfx::Rect());
  }
}
