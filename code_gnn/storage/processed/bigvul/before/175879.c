netdutils::Status XfrmController::flushInterfaces() {
 const auto& ifaces = InterfaceController::getIfaceNames();
    RETURN_IF_NOT_OK(ifaces);
 const String8 ifPrefix8 = String8(INetd::IPSEC_INTERFACE_PREFIX().string());

 for (const std::string& iface : ifaces.value()) {
 int status = 0;
 if (!iface.compare(0, ifPrefix8.length(), ifPrefix8.c_str()) &&
 (status = removeVirtualTunnelInterface(iface)) < 0) {
            ALOGE("Failed to delete ipsec tunnel %s.", iface.c_str());
 return netdutils::statusFromErrno(status, "Failed to remove ipsec tunnel.");
 }
 }
 return netdutils::status::ok;
}
