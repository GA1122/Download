void btif_thread_post(thread_fn func, void *context) {
    thread_post(bt_jni_workqueue_thread, func, context);
}
