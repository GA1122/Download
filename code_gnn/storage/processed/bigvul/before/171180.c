sp<IRemoteDisplay> MediaPlayerService::listenForRemoteDisplay(
 const String16 &opPackageName,
 const sp<IRemoteDisplayClient>& client, const String8& iface) {
 if (!checkPermission("android.permission.CONTROL_WIFI_DISPLAY")) {
 return NULL;
 }

 return new RemoteDisplay(opPackageName, client, iface.string());
}
