bool PPResultAndExceptionToNPResult::SetResult(PP_Var result) {
  DCHECK(!checked_exception_);   
  DCHECK(np_result_);   

  checked_exception_ = true;

  if (has_exception()) {
    ThrowException();
    success_ = false;
  } else if (!PPVarToNPVariant(result, np_result_)) {
    WebBindings::setException(object_var_, kInvalidPluginValue);
    success_ = false;
  } else {
    success_ = true;
  }

  Var::PluginReleasePPVar(result);
  return success_;
}
