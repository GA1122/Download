bool jspCheckStackPosition() {
  if (jsuGetFreeStack() < 512) {  
    jsExceptionHere(JSET_ERROR, "Too much recursion - the stack is about to overflow");
    jspSetInterrupted(true);
    return false;
  }
  return true;
}
