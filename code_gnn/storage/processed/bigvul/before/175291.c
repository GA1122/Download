static void freeDebugCallbackArgs(int number, char **args) {
 for (int i = 0; i < number; i++) {
 if (args[i] != NULL) {
            free(args[i]);
 }
 }
    free(args);
}
