bool ACodec::BaseState::checkOMXMessage(const sp<AMessage> &msg) {
 if (mCodec->mNode == 0) {
        ALOGI("ignoring message as already freed component: %s",
                msg->debugString().c_str());
 return false;
 }

    IOMX::node_id nodeID;
    CHECK(msg->findInt32("node", (int32_t*)&nodeID));
 if (nodeID != mCodec->mNode) {
        ALOGE("Unexpected message for nodeID: %u, should have been %u", nodeID, mCodec->mNode);
 return false;
 }
 return true;
}
