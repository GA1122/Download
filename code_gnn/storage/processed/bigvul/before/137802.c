TabAlertState GetTabAlertStateForContents(content::WebContents* contents) {
  if (!contents)
    return TabAlertState::NONE;

  scoped_refptr<MediaStreamCaptureIndicator> indicator =
      MediaCaptureDevicesDispatcher::GetInstance()->
          GetMediaStreamCaptureIndicator();
  if (indicator.get()) {
    if (indicator->IsBeingMirrored(contents))
      return TabAlertState::TAB_CAPTURING;
    if (indicator->IsCapturingUserMedia(contents))
      return TabAlertState::MEDIA_RECORDING;
  }

  if (contents->IsConnectedToBluetoothDevice())
    return TabAlertState::BLUETOOTH_CONNECTED;

  UsbTabHelper* usb_tab_helper = UsbTabHelper::FromWebContents(contents);
  if (usb_tab_helper && usb_tab_helper->IsDeviceConnected())
    return TabAlertState::USB_CONNECTED;

  if (contents->IsAudioMuted())
    return TabAlertState::AUDIO_MUTING;
  if (contents->WasRecentlyAudible())
    return TabAlertState::AUDIO_PLAYING;

  return TabAlertState::NONE;
}
