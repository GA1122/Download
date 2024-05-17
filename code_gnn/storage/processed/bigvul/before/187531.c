 void CameraSource::releaseQueuedFrames() {
 
      List<sp<IMemory> >::iterator it;
      while (!mFramesReceived.empty()) {
          it = mFramesReceived.begin();
//          
//         adjustOutgoingANWBuffer(it->get());
          releaseRecordingFrame(*it);
          mFramesReceived.erase(it);
          ++mNumFramesDropped;
  }
 }