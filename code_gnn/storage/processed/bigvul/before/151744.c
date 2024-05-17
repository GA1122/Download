void Browser::ShowValidationMessage(content::WebContents* web_contents,
                                    const gfx::Rect& anchor_in_root_view,
                                    const base::string16& main_text,
                                    const base::string16& sub_text) {
  if (!web_contents->GetTopLevelNativeWindow())
    return;
  validation_message_bubble_ =
      TabDialogs::FromWebContents(web_contents)
          ->ShowValidationMessage(anchor_in_root_view, main_text, sub_text);
}
