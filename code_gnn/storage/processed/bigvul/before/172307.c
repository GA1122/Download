static void onRttResults(wifi_request_id id, unsigned num_results, wifi_rtt_result* results[]) {

 JNIHelper helper(mVM);

    ALOGD("onRttResults called, vm = %p, obj = %p", mVM, mCls);

 JNIObject<jobjectArray> rttResults = helper.newObjectArray(
            num_results, "android/net/wifi/RttManager$RttResult", NULL);
 if (rttResults == NULL) {
        ALOGE("Error in allocating array");
 return;
 }

 for (unsigned i = 0; i < num_results; i++) {

        wifi_rtt_result *result = results[i];

 JNIObject<jobject> rttResult = helper.createObject("android/net/wifi/RttManager$RttResult");
 if (rttResult == NULL) {
            ALOGE("Error in creating rtt result");
 return;
 }

 char bssid[32];
        sprintf(bssid, "%02x:%02x:%02x:%02x:%02x:%02x", result->addr[0], result->addr[1],
            result->addr[2], result->addr[3], result->addr[4], result->addr[5]);

        helper.setStringField(rttResult, "bssid", bssid);
        helper.setIntField( rttResult, "burstNumber",              result->burst_num);
        helper.setIntField( rttResult, "measurementFrameNumber",   result->measurement_number);
        helper.setIntField( rttResult, "successMeasurementFrameNumber",   result->success_number);
        helper.setIntField(rttResult, "frameNumberPerBurstPeer",   result->number_per_burst_peer);
        helper.setIntField( rttResult, "status",                   result->status);
        helper.setIntField( rttResult, "measurementType",          result->type);
        helper.setIntField(rttResult, "retryAfterDuration",       result->retry_after_duration);
        helper.setLongField(rttResult, "ts",                       result->ts);
        helper.setIntField( rttResult, "rssi",                     result->rssi);
        helper.setIntField( rttResult, "rssiSpread",               result->rssi_spread);
        helper.setIntField( rttResult, "txRate",                   result->tx_rate.bitrate);
        helper.setIntField( rttResult, "rxRate",                   result->rx_rate.bitrate);
        helper.setLongField(rttResult, "rtt",                      result->rtt);
        helper.setLongField(rttResult, "rttStandardDeviation",     result->rtt_sd);
        helper.setIntField( rttResult, "distance",                 result->distance);
        helper.setIntField( rttResult, "distanceStandardDeviation", result->distance_sd);
        helper.setIntField( rttResult, "distanceSpread",           result->distance_spread);
        helper.setIntField( rttResult, "burstDuration",             result->burst_duration);
        helper.setIntField( rttResult, "negotiatedBurstNum",      result->negotiated_burst_num);

 JNIObject<jobject> LCI = helper.createObject(
 "android/net/wifi/RttManager$WifiInformationElement");
 if (result->LCI != NULL && result->LCI->len > 0) {
            ALOGD("Add LCI in result");
            helper.setByteField(LCI, "id", result->LCI->id);
 JNIObject<jbyteArray> elements = helper.newByteArray(result->LCI->len);
            jbyte *bytes = (jbyte *)&(result->LCI->data[0]);
            helper.setByteArrayRegion(elements, 0, result->LCI->len, bytes);
            helper.setObjectField(LCI, "data", "[B", elements);
 } else {
            ALOGD("No LCI in result");
            helper.setByteField(LCI, "id", (byte)(0xff));
 }
        helper.setObjectField(rttResult, "LCI",
 "Landroid/net/wifi/RttManager$WifiInformationElement;", LCI);

 JNIObject<jobject> LCR = helper.createObject(
 "android/net/wifi/RttManager$WifiInformationElement");
 if (result->LCR != NULL && result->LCR->len > 0) {
            ALOGD("Add LCR in result");
            helper.setByteField(LCR, "id",           result->LCR->id);
 JNIObject<jbyteArray> elements = helper.newByteArray(result->LCI->len);
            jbyte *bytes = (jbyte *)&(result->LCR->data[0]);
            helper.setByteArrayRegion(elements, 0, result->LCI->len, bytes);
            helper.setObjectField(LCR, "data", "[B", elements);
 } else {
            ALOGD("No LCR in result");
            helper.setByteField(LCR, "id", (byte)(0xff));
 }
        helper.setObjectField(rttResult, "LCR",
 "Landroid/net/wifi/RttManager$WifiInformationElement;", LCR);

        helper.setObjectArrayElement(rttResults, i, rttResult);
 }

    helper.reportEvent(mCls, "onRttResults", "(I[Landroid/net/wifi/RttManager$RttResult;)V",
        id, rttResults.get());
}
