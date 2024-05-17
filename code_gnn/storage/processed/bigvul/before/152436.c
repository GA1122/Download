void RenderFrameImpl::OnImeCommitText(const base::string16& text,
                                      const gfx::Range& replacement_range,
                                      int relative_cursor_pos) {
  HandlePepperImeCommit(text);
}
