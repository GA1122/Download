 void PasswordInputType::OnAttachWithLayoutObject() {
   GetElement().GetDocument().IncrementPasswordCount();
 }
