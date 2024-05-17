void Browser::AddNewContents(WebContents* source,
                             WebContents* new_contents,
                             WindowOpenDisposition disposition,
                             const gfx::Rect& initial_pos,
                             bool user_gesture,
                             bool* was_blocked) {
  chrome::AddWebContents(this, source, new_contents, disposition, initial_pos,
                         user_gesture, was_blocked);
}
