void jspSoftInit() {
  execInfo.root = jsvFindOrCreateRoot();
  execInfo.hiddenRoot = jsvObjectGetChild(execInfo.root, JS_HIDDEN_CHAR_STR, JSV_OBJECT);
  execInfo.execute = EXEC_YES;
}
