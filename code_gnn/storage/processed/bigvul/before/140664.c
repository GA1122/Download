bool OutOfProcessInstance::StartFind(const std::string& text,
                                                 bool case_sensitive) {
  engine_->StartFind(text.c_str(), case_sensitive);
  return true;
}
