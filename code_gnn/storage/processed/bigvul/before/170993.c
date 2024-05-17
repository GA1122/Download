void AudioFlinger::EffectModule::dump(int fd, const Vector<String16>& args __unused)
{
 const size_t SIZE = 256;
 char buffer[SIZE];
 String8 result;

    snprintf(buffer, SIZE, "\tEffect ID %d:\n", mId);
    result.append(buffer);

 bool locked = AudioFlinger::dumpTryLock(mLock);
 if (!locked) {
        result.append("\t\tCould not lock Fx mutex:\n");
 }

    result.append("\t\tSession Status State Engine:\n");
    snprintf(buffer, SIZE, "\t\t%05d   %03d    %03d   %p\n",
            mSessionId, mStatus, mState, mEffectInterface);
    result.append(buffer);

    result.append("\t\tDescriptor:\n");
    snprintf(buffer, SIZE, "\t\t- UUID: %08X-%04X-%04X-%04X-%02X%02X%02X%02X%02X%02X\n",
            mDescriptor.uuid.timeLow, mDescriptor.uuid.timeMid, mDescriptor.uuid.timeHiAndVersion,
            mDescriptor.uuid.clockSeq, mDescriptor.uuid.node[0], mDescriptor.uuid.node[1],
                    mDescriptor.uuid.node[2],
            mDescriptor.uuid.node[3],mDescriptor.uuid.node[4],mDescriptor.uuid.node[5]);
    result.append(buffer);
    snprintf(buffer, SIZE, "\t\t- TYPE: %08X-%04X-%04X-%04X-%02X%02X%02X%02X%02X%02X\n",
                mDescriptor.type.timeLow, mDescriptor.type.timeMid,
                    mDescriptor.type.timeHiAndVersion,
                mDescriptor.type.clockSeq, mDescriptor.type.node[0], mDescriptor.type.node[1],
                    mDescriptor.type.node[2],
                mDescriptor.type.node[3],mDescriptor.type.node[4],mDescriptor.type.node[5]);
    result.append(buffer);
    snprintf(buffer, SIZE, "\t\t- apiVersion: %08X\n\t\t- flags: %08X (%s)\n",
            mDescriptor.apiVersion,
            mDescriptor.flags,
            effectFlagsToString(mDescriptor.flags).string());
    result.append(buffer);
    snprintf(buffer, SIZE, "\t\t- name: %s\n",
            mDescriptor.name);
    result.append(buffer);
    snprintf(buffer, SIZE, "\t\t- implementor: %s\n",
            mDescriptor.implementor);
    result.append(buffer);

    result.append("\t\t- Input configuration:\n");
    result.append("\t\t\tFrames  Smp rate Channels Format Buffer\n");
    snprintf(buffer, SIZE, "\t\t\t%05zu   %05d    %08x %6d (%s) %p\n",
            mConfig.inputCfg.buffer.frameCount,
            mConfig.inputCfg.samplingRate,
            mConfig.inputCfg.channels,
            mConfig.inputCfg.format,
            formatToString((audio_format_t)mConfig.inputCfg.format),
            mConfig.inputCfg.buffer.raw);
    result.append(buffer);

    result.append("\t\t- Output configuration:\n");
    result.append("\t\t\tBuffer     Frames  Smp rate Channels Format\n");
    snprintf(buffer, SIZE, "\t\t\t%p %05zu   %05d    %08x %d (%s)\n",
            mConfig.outputCfg.buffer.raw,
            mConfig.outputCfg.buffer.frameCount,
            mConfig.outputCfg.samplingRate,
            mConfig.outputCfg.channels,
            mConfig.outputCfg.format,
            formatToString((audio_format_t)mConfig.outputCfg.format));
    result.append(buffer);

    snprintf(buffer, SIZE, "\t\t%zu Clients:\n", mHandles.size());
    result.append(buffer);
    result.append("\t\t\t  Pid Priority Ctrl Locked client server\n");
 for (size_t i = 0; i < mHandles.size(); ++i) {
 EffectHandle *handle = mHandles[i];
 if (handle != NULL && !handle->destroyed_l()) {
            handle->dumpToBuffer(buffer, SIZE);
            result.append(buffer);
 }
 }

    write(fd, result.string(), result.length());

 if (locked) {
        mLock.unlock();
 }
}
