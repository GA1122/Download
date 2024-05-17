static void setLogLevel(int level) {
    android_atomic_write(level, &gLogLevel);
}
