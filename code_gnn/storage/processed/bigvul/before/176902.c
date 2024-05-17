void InputDispatcher::ConfigurationChangedEntry::appendDescription(String8& msg) const {
    msg.append("ConfigurationChangedEvent(), policyFlags=0x%08x",
            policyFlags);
}
