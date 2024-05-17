bool HostNPScriptObject::HasProperty(const std::string& property_name) {
  VLOG(2) << "HasProperty " << property_name;
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);
  return (property_name == kAttrNameAccessCode ||
          property_name == kAttrNameAccessCodeLifetime ||
          property_name == kAttrNameClient ||
          property_name == kAttrNameState ||
          property_name == kAttrNameLogDebugInfo ||
          property_name == kAttrNameOnNatTraversalPolicyChanged ||
          property_name == kAttrNameOnStateChanged ||
          property_name == kAttrNameDisconnected ||
          property_name == kAttrNameStarting ||
          property_name == kAttrNameRequestedAccessCode ||
          property_name == kAttrNameReceivedAccessCode ||
          property_name == kAttrNameConnected ||
          property_name == kAttrNameDisconnecting ||
          property_name == kAttrNameError);
}
