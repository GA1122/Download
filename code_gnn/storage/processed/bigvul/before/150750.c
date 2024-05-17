BluetoothDeviceChooserController::BluetoothDeviceChooserController(
    WebBluetoothServiceImpl* web_bluetooth_service,
    RenderFrameHost* render_frame_host,
    device::BluetoothAdapter* adapter)
    : adapter_(adapter),
      web_bluetooth_service_(web_bluetooth_service),
      render_frame_host_(render_frame_host),
      web_contents_(WebContents::FromRenderFrameHost(render_frame_host_)),
      discovery_session_timer_(
          FROM_HERE,
          base::TimeDelta::FromSeconds(scan_duration_),
          base::Bind(&BluetoothDeviceChooserController::StopDeviceDiscovery,
                     base::Unretained(this))) {
  CHECK(adapter_);
}
