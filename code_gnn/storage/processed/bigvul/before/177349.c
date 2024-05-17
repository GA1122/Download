status_t ACodec::waitForFence(int fd, const char *dbg ) {
 status_t res = OK;
 if (fd >= 0) {
        sp<Fence> fence = new Fence(fd);
        res = fence->wait(IOMX::kFenceTimeoutMs);
        ALOGW_IF(res != OK, "FENCE TIMEOUT for %d in %s", fd, dbg);
 }
 return res;
}
