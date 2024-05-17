void btif_sendmsg(void *p_msg)
{
    thread_post(bt_jni_workqueue_thread, bt_jni_msg_ready, p_msg);
}
