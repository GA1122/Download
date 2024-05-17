uint32_t OMXCodec::getComponentQuirks(
 const sp<MediaCodecInfo> &info) {
 uint32_t quirks = 0;
 if (info->hasQuirk("requires-allocate-on-input-ports")) {
        quirks |= kRequiresAllocateBufferOnInputPorts;
 }
 if (info->hasQuirk("requires-allocate-on-output-ports")) {
        quirks |= kRequiresAllocateBufferOnOutputPorts;
 }
 if (info->hasQuirk("output-buffers-are-unreadable")) {
        quirks |= kOutputBuffersAreUnreadable;
 }

 return quirks;
}
