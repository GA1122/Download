static jboolean doBooleanCommand(JNIEnv* env, jstring javaCommand) {
 char reply[REPLY_BUF_SIZE];
 if (!doCommand(env, javaCommand, reply, sizeof(reply))) {
 return JNI_FALSE;
 }
    jboolean result = (strcmp(reply, "OK") == 0);
 if (!result) {
 ScopedUtfChars command(env, javaCommand);
        ALOGI("command '%s' returned '%s", command.c_str(), reply);
 }
 return result;
}
