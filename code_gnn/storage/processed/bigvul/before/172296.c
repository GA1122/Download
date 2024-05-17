static jint doIntCommand(JNIEnv* env, jstring javaCommand) {
 char reply[REPLY_BUF_SIZE];
 if (!doCommand(env, javaCommand, reply, sizeof(reply))) {
 return -1;
 }
 return static_cast<jint>(atoi(reply));
}
