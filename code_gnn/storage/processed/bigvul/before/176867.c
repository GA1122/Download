static int readEnergyInfo()
{
    ALOGV("%s:",__FUNCTION__);
    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;
 int ret = sBluetoothInterface->read_energy_info();
    result = (ret == BT_STATUS_SUCCESS) ? JNI_TRUE : JNI_FALSE;
 return result;
}
