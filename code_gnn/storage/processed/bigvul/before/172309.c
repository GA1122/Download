static void onScanResultsAvailable(wifi_request_id id, unsigned num_results) {

 JNIHelper helper(mVM);


    helper.reportEvent(mCls, "onScanResultsAvailable", "(I)V", id);
}
