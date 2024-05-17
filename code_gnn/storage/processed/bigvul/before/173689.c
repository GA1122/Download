bool ATSParser::Program::switchPIDs(const Vector<StreamInfo> &infos) {
 bool success = false;

 if (mStreams.size() == infos.size()) {
 size_t i;
 KeyedVector<int32_t, Vector<int32_t> > oldType2PIDs, newType2PIDs;
 for (i = 0; i < mStreams.size(); ++i) {
 ssize_t index = oldType2PIDs.indexOfKey(mStreams[i]->type());
 if (index < 0) {
                oldType2PIDs.add(mStreams[i]->type(), Vector<int32_t>());
 }
            oldType2PIDs.editValueFor(mStreams[i]->type()).push_back(mStreams[i]->pid());
 }
 for (i = 0; i < infos.size(); ++i) {
 ssize_t index = newType2PIDs.indexOfKey(infos[i].mType);
 if (index < 0) {
                newType2PIDs.add(infos[i].mType, Vector<int32_t>());
 }
            newType2PIDs.editValueFor(infos[i].mType).push_back(infos[i].mPID);
 }

 if (oldType2PIDs.size() == newType2PIDs.size()) {
            success = true;
 for (i = 0; i < oldType2PIDs.size(); ++i) {
 if (oldType2PIDs.keyAt(i) != newType2PIDs.keyAt(i)
 || oldType2PIDs[i].size() != newType2PIDs[i].size()) {
                     success = false;
 break;
 }
 }
 }

 if (success) {
 KeyedVector<int32_t, sp<Stream> > temp;
 for (i = 0; i < mStreams.size(); ++i) {
                 temp.add(mStreams.keyAt(i), mStreams.editValueAt(i));
 }

            mStreams.clear();
 for (i = 0; i < temp.size(); ++i) {
 ssize_t index = newType2PIDs.indexOfKey(temp[i]->type());
 if (index < 0) {
 return false;
 }
 Vector<int32_t> &newPIDs = newType2PIDs.editValueAt(index);
 if (newPIDs.isEmpty()) {
 return false;
 }

 Vector<int32_t>::iterator it = newPIDs.begin();

                temp.editValueAt(i)->setPID(*it);
                mStreams.add(temp[i]->pid(), temp.editValueAt(i));

                newPIDs.erase(it);
 }
 }
 }
 return success;
}
