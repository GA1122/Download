static jboolean android_net_wifi_startScan(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject settings) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    wifi_scan_cmd_params params;
    memset(&params, 0, sizeof(params));

    params.base_period = helper.getIntField(settings, "base_period_ms");
    params.max_ap_per_scan = helper.getIntField(settings, "max_ap_per_scan");
    params.report_threshold_percent = helper.getIntField(settings, "report_threshold_percent");
    params.report_threshold_num_scans = helper.getIntField(settings, "report_threshold_num_scans");

    ALOGD("Initialized common fields %d, %d, %d, %d", params.base_period, params.max_ap_per_scan,
            params.report_threshold_percent, params.report_threshold_num_scans);

 const char *bucket_array_type = "[Lcom/android/server/wifi/WifiNative$BucketSettings;";
 const char *channel_array_type = "[Lcom/android/server/wifi/WifiNative$ChannelSettings;";

    params.num_buckets = helper.getIntField(settings, "num_buckets");


 for (int i = 0; i < params.num_buckets; i++) {
 JNIObject<jobject> bucket = helper.getObjectArrayField(
                settings, "buckets", bucket_array_type, i);

        params.buckets[i].bucket = helper.getIntField(bucket, "bucket");
        params.buckets[i].band = (wifi_band) helper.getIntField(bucket, "band");
        params.buckets[i].period = helper.getIntField(bucket, "period_ms");

 int report_events = helper.getIntField(bucket, "report_events");
        params.buckets[i].report_events = report_events;

        ALOGD("bucket[%d] = %d:%d:%d:%d", i, params.buckets[i].bucket,
                params.buckets[i].band, params.buckets[i].period, report_events);

        params.buckets[i].num_channels = helper.getIntField(bucket, "num_channels");

 for (int j = 0; j < params.buckets[i].num_channels; j++) {
 JNIObject<jobject> channel = helper.getObjectArrayField(
                    bucket, "channels", channel_array_type, j);

            params.buckets[i].channels[j].channel = helper.getIntField(channel, "frequency");
            params.buckets[i].channels[j].dwellTimeMs = helper.getIntField(channel, "dwell_time_ms");

 bool passive = helper.getBoolField(channel, "passive");
            params.buckets[i].channels[j].passive = (passive ? 1 : 0);

 }
 }


    wifi_scan_result_handler handler;
    memset(&handler, 0, sizeof(handler));
    handler.on_scan_results_available = &onScanResultsAvailable;
    handler.on_full_scan_result = &onFullScanResult;
    handler.on_scan_event = &onScanEvent;

 return hal_fn.wifi_start_gscan(id, handle, params, handler) == WIFI_SUCCESS;
}
