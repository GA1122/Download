bt_status_t btif_init_bluetooth() {
  bte_main_boot_entry();

  
  memset(&btif_local_bd_addr, 0, sizeof(bt_bdaddr_t));
  btif_fetch_local_bdaddr(&btif_local_bd_addr);

  bt_jni_workqueue_thread = thread_new(BT_JNI_WORKQUEUE_NAME);
 if (bt_jni_workqueue_thread == NULL) {
    LOG_ERROR("%s Unable to create thread %s", __func__, BT_JNI_WORKQUEUE_NAME);
 goto error_exit;
 }

  btif_transfer_context(btif_jni_associate, 0, NULL, 0, NULL);

 return BT_STATUS_SUCCESS;

error_exit:;
     thread_free(bt_jni_workqueue_thread);

     bt_jni_workqueue_thread = NULL;

 return BT_STATUS_FAIL;
}
