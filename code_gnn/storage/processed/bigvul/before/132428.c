void UsbDeviceImpl::CheckUsbAccess(const ResultCallback& callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  chromeos::PermissionBrokerClient* client =
      chromeos::DBusThreadManager::Get()->GetPermissionBrokerClient();
  DCHECK(client) << "Could not get permission broker client.";
  client->CheckPathAccess(device_path_, callback);
}
