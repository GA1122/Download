bool PDFScriptableObject::HasMethod(const pp::Var& name, pp::Var* exception) {
  return instance_->HasScriptableMethod(name, exception);
}
