void HTMLInputElement::EndEditing() {
  DCHECK(GetDocument().IsActive());
  if (!GetDocument().IsActive())
    return;

  if (!IsTextField())
    return;

  LocalFrame* frame = GetDocument().GetFrame();
  frame->GetSpellChecker().DidEndEditingOnTextField(this);
   frame->GetPage()->GetChromeClient().DidEndEditingOnTextField(*this);
 }
