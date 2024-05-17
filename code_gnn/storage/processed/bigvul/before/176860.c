static int get_properties(int num_properties, bt_property_t *properties, jintArray *types,
                        jobjectArray *props) {
    jbyteArray propVal;
 for (int i = 0; i < num_properties; i++) {
        propVal = callbackEnv->NewByteArray(properties[i].len);
 if (propVal == NULL) goto Fail;

        callbackEnv->SetByteArrayRegion(propVal, 0, properties[i].len,
 (jbyte*)properties[i].val);
        callbackEnv->SetObjectArrayElement(*props, i, propVal);
        callbackEnv->DeleteLocalRef(propVal);
        callbackEnv->SetIntArrayRegion(*types, i, 1, (jint *)&properties[i].type);
 }
 return 0;
Fail:
 if (propVal) callbackEnv->DeleteLocalRef(propVal);
    ALOGE("Error while allocation of array in %s", __FUNCTION__);
 return -1;
}
