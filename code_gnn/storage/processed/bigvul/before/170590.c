int16_t VolumeConvertStereoPosition(int16_t position){
 int16_t convertedPosition = 0;

    convertedPosition = (int16_t)(((float)position/1000)*96);
 return convertedPosition;

}
