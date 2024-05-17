static jboolean android_net_wifi_requestRange(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject params) {

 JNIHelper helper(env);

    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("sending rtt request [%d] = %p", id, handle);

    wifi_rtt_config configs[MaxRttConfigs];
    memset(&configs, 0, sizeof(configs));

 int len = helper.getArrayLength((jobjectArray)params);
 if (len > MaxRttConfigs) {
 return false;
 }

 for (int i = 0; i < len; i++) {

 JNIObject<jobject> param = helper.getObjectArrayElement((jobjectArray)params, i);
 if (param == NULL) {
            ALOGD("could not get element %d", i);
 continue;
 }

        wifi_rtt_config &config = configs[i];

        parseMacAddress(env, param, config.addr);
        config.type = (wifi_rtt_type)helper.getIntField(param, "requestType");
        config.peer = (rtt_peer_type)helper.getIntField(param, "deviceType");
        config.channel.center_freq = helper.getIntField(param, "frequency");
        config.channel.width = (wifi_channel_width) helper.getIntField(param, "channelWidth");
        config.channel.center_freq0 = helper.getIntField(param, "centerFreq0");
        config.channel.center_freq1 = helper.getIntField(param, "centerFreq1");

        config.num_burst = helper.getIntField(param, "numberBurst");
        config.burst_period = (unsigned) helper.getIntField(param, "interval");
        config.num_frames_per_burst = (unsigned) helper.getIntField(param, "numSamplesPerBurst");
        config.num_retries_per_rtt_frame = (unsigned) helper.getIntField(param,
 "numRetriesPerMeasurementFrame");
        config.num_retries_per_ftmr = (unsigned) helper.getIntField(param, "numRetriesPerFTMR");
        config.LCI_request = helper.getBoolField(param, "LCIRequest") ? 1 : 0;
        config.LCR_request = helper.getBoolField(param, "LCRRequest") ? 1 : 0;
        config.burst_duration = (unsigned) helper.getIntField(param, "burstTimeout");
        config.preamble = (wifi_rtt_preamble) helper.getIntField(param, "preamble");
        config.bw = (wifi_rtt_bw) helper.getIntField(param, "bandwidth");

        ALOGD("RTT request destination %d: type is %d, peer is %d, bw is %d, center_freq is %d ", i,
                config.type,config.peer, config.channel.width,  config.channel.center_freq);
        ALOGD("center_freq0 is %d, center_freq1 is %d, num_burst is %d,interval is %d",
                config.channel.center_freq0, config.channel.center_freq1, config.num_burst,
                config.burst_period);
        ALOGD("frames_per_burst is %d, retries of measurement frame is %d, retries_per_ftmr is %d",
                config.num_frames_per_burst, config.num_retries_per_rtt_frame,
                config.num_retries_per_ftmr);
        ALOGD("LCI_requestis %d, LCR_request is %d,  burst_timeout is %d, preamble is %d, bw is %d",
                config.LCI_request, config.LCR_request, config.burst_duration, config.preamble,
                config.bw);
 }

    wifi_rtt_event_handler handler;
    handler.on_rtt_results = &onRttResults;

 return hal_fn.wifi_rtt_range_request(id, handle, len, configs, handler) == WIFI_SUCCESS;
}
