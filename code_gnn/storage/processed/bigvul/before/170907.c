void OMXNodeInstance::onMessages(std::list<omx_message> &messages) {
 for (std::list<omx_message>::iterator it = messages.begin(); it != messages.end(); ) {
 if (handleMessage(*it)) {
            messages.erase(it++);
 } else {
 ++it;
 }
 }

 if (!messages.empty()) {
        mObserver->onMessages(messages);
 }
}
