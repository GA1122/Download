String Document::suborigin() const {
  return GetSecurityOrigin()->HasSuborigin()
             ? GetSecurityOrigin()->GetSuborigin()->GetName()
             : String();
}
