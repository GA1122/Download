static jstring doStringCommand(JNIEnv* env, jstring javaCommand) {
 char reply[REPLY_BUF_SIZE];
 if (!doCommand(env, javaCommand, reply, sizeof(reply))) {
 return NULL;
 }
 return env->NewStringUTF(reply);
}
