void OutOfProcessInstance::OnPrint(int32_t) {
  pp::PDF::Print(this);
}
