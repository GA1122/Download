void ID3::Iterator::getID(String8 *id) const {
    id->setTo("");

 if (mFrameData == NULL) {
 return;
 }

 if (mParent.mVersion == ID3_V2_2) {
        id->setTo((const char *)&mParent.mData[mOffset], 3);
 } else if (mParent.mVersion == ID3_V2_3 || mParent.mVersion == ID3_V2_4) {
        id->setTo((const char *)&mParent.mData[mOffset], 4);
 } else {
        CHECK(mParent.mVersion == ID3_V1 || mParent.mVersion == ID3_V1_1);

 switch (mOffset) {
 case 3:
                id->setTo("TT2");
 break;
 case 33:
                id->setTo("TP1");
 break;
 case 63:
                id->setTo("TAL");
 break;
 case 93:
                id->setTo("TYE");
 break;
 case 97:
                id->setTo("COM");
 break;
 case 126:
                id->setTo("TRK");
 break;
 case 127:
                id->setTo("TCO");
 break;
 default:
                CHECK(!"should not be here.");
 break;
 }
 }
}
