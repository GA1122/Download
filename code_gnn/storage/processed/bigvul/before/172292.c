static int compare_scan_result_timestamp(const void *v1, const void *v2) {
 const wifi_scan_result *result1 = static_cast<const wifi_scan_result *>(v1);
 const wifi_scan_result *result2 = static_cast<const wifi_scan_result *>(v2);
 return result1->ts - result2->ts;
}
