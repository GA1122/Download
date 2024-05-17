void PasswordInputType::OnDetachWithLayoutObject() {
  GetElement().GetDocument().DecrementPasswordCount();
}
