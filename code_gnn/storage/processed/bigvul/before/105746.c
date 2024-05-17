bool PPResultAndExceptionToNPResult::CheckExceptionForNoResult() {
  DCHECK(!checked_exception_);   
  DCHECK(!np_result_);   

  checked_exception_ = true;

  if (has_exception()) {
    ThrowException();
    success_ = false;
    return false;
  }
  success_ = true;
  return true;
}
