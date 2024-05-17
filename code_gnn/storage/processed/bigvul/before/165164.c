String HTMLFormElement::action() const {
  Document& document = GetDocument();
  KURL action_url = document.CompleteURL(attributes_.Action().IsEmpty()
                                             ? document.Url().GetString()
                                             : attributes_.Action());
  return action_url.GetString();
}
