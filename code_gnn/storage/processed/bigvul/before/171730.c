bt_status_t btif_shutdown_bluetooth(void)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

    btif_transfer_context(btif_jni_disassociate, 0, NULL, 0, NULL);

    btif_queue_release();

    thread_free(bt_jni_workqueue_thread);
    bt_jni_workqueue_thread = NULL;

    bte_main_shutdown();

    btif_dut_mode = 0;

    BTIF_TRACE_DEBUG("%s done", __FUNCTION__);

 return BT_STATUS_SUCCESS;
}
