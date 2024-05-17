void IPCThreadState::processPendingDerefs()
{
 if (mIn.dataPosition() >= mIn.dataSize()) {
 size_t numPending = mPendingWeakDerefs.size();
 if (numPending > 0) {
 for (size_t i = 0; i < numPending; i++) {
 RefBase::weakref_type* refs = mPendingWeakDerefs[i];
                refs->decWeak(mProcess.get());
 }
            mPendingWeakDerefs.clear();
 }

        numPending = mPendingStrongDerefs.size();
 if (numPending > 0) {
 for (size_t i = 0; i < numPending; i++) {
 BBinder* obj = mPendingStrongDerefs[i];
                obj->decStrong(mProcess.get());
 }
            mPendingStrongDerefs.clear();
 }
 }
}
