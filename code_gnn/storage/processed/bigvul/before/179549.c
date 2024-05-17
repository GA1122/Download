 static int logi_dj_raw_event(struct hid_device *hdev,
 			     struct hid_report *report, u8 *data,
 			     int size)
 {
 	struct dj_receiver_dev *djrcv_dev = hid_get_drvdata(hdev);
 	struct dj_report *dj_report = (struct dj_report *) data;
 	unsigned long flags;
 	bool report_processed = false;
 
 	dbg_hid("%s, size:%d\n", __func__, size);
 
 	 
// 	if ((dj_report->device_index < DJ_DEVICE_INDEX_MIN) ||
// 	    (dj_report->device_index > DJ_DEVICE_INDEX_MAX)) {
// 		dev_err(&hdev->dev, "%s: invalid device index:%d\n",
// 				__func__, dj_report->device_index);
// 		return false;
// 	}
  
  	spin_lock_irqsave(&djrcv_dev->lock, flags);
  	if (dj_report->report_id == REPORT_ID_DJ_SHORT) {
 		switch (dj_report->report_type) {
 		case REPORT_TYPE_NOTIF_DEVICE_PAIRED:
 		case REPORT_TYPE_NOTIF_DEVICE_UNPAIRED:
 			logi_dj_recv_queue_notification(djrcv_dev, dj_report);
 			break;
 		case REPORT_TYPE_NOTIF_CONNECTION_STATUS:
 			if (dj_report->report_params[CONNECTION_STATUS_PARAM_STATUS] ==
 			    STATUS_LINKLOSS) {
 				logi_dj_recv_forward_null_report(djrcv_dev, dj_report);
 			}
 			break;
 		default:
 			logi_dj_recv_forward_report(djrcv_dev, dj_report);
 		}
 		report_processed = true;
 	}
 	spin_unlock_irqrestore(&djrcv_dev->lock, flags);
 
 	return report_processed;
 }