static void dispatchDataCall(Parcel& p, RequestInfo *pRI) {
 const int numParamsRilV3 = 6;

 int pos = p.dataPosition();

 int numParams = p.readInt32();
 if (s_callbacks.version < 4 && numParams > numParamsRilV3) {
 Parcel p2;
      p2.appendFrom(&p, 0, pos);
      p2.writeInt32(numParamsRilV3);
 for(int i = 0; i < numParamsRilV3; i++) {
        p2.writeString16(p.readString16());
 }
      p2.setDataPosition(pos);
      dispatchStrings(p2, pRI);
 } else {
      p.setDataPosition(pos);
      dispatchStrings(p, pRI);
 }
}
