void jspSoftKill() {
  jsvUnLock(execInfo.hiddenRoot);
  execInfo.hiddenRoot = 0;
  jsvUnLock(execInfo.root);
  execInfo.root = 0;
}