static void writeExifTagAndData(int tag,
 int format,
 long components,
 long value,
 int valueInString,
 char* Buffer,
 int* DirIndex,
 int* DataWriteIndex) {
 void* componentsPosition = NULL;  

 Put16u(Buffer+ (*DirIndex), tag);  
 Put16u(Buffer+(*DirIndex) + 2, format);  
 if (format == FMT_STRING && components == -1) {
        components = strlen((char*)value) + 1;  
 if (components & 1) ++components;  
 } else if (format == FMT_SSHORT && components == -1) {
        components = 1;
 }
 if (format == FMT_UNDEFINED && components == -1) {
 if(memcmp((char*)value, ExifAsciiPrefix, sizeof(ExifAsciiPrefix)) == 0) {
            components = sizeof(ExifAsciiPrefix) +
                         strlen((char*)value + sizeof(ExifAsciiPrefix)) + 1;
 if (components & 1) ++components;  
 }
 }
 Put32u(Buffer+(*DirIndex) + 4, components);  
    componentsPosition = Buffer+(*DirIndex) + 4;  
    printf("# components: %ld", components);
 if (format == FMT_STRING) {
 if (components <= 4) {
            strcpy(Buffer+(*DirIndex) + 8, (char*)value);
 } else {
 Put32u(Buffer+(*DirIndex) + 8, (*DataWriteIndex)-8);  
            printf("copying value %s to %d", (char*)value, (*DataWriteIndex));
            strncpy(Buffer+(*DataWriteIndex), (char*)value, components);
 (*DataWriteIndex) += components;
 }
 } else if ((format == FMT_UNDEFINED) &&
 (memcmp((char*)value, ExifAsciiPrefix, sizeof(ExifAsciiPrefix)) == 0)) {
 if (components <= 4) {
            memcpy(Buffer+(*DirIndex) + 8, (char*)value, components);
 } else {
 Put32u(Buffer+(*DirIndex) + 8, (*DataWriteIndex)-8);  
            printf("copying %s to %d", (char*)value + sizeof(ExifAsciiPrefix), (*DataWriteIndex));
            memcpy(Buffer+(*DataWriteIndex), (char*)value, components);
 (*DataWriteIndex) += components;
 }
 } else if (!valueInString) {
 Put32u(Buffer+(*DirIndex) + 8, value);  
 } else {
 Put32u(Buffer+(*DirIndex) + 8, (*DataWriteIndex)-8);  
 char* curElement = strtok((char*)value, ",:");
 int i;

 for (i = 0; ((i < components) || (components == -1)) && curElement != NULL; i++) {
#ifdef SUPERDEBUG
            printf("processing component %s format %s", curElement, formatStr(format));
#endif
 if (format == FMT_URATIONAL) {
 unsigned int numerator;
 unsigned int denominator;
 char* separator = strchr(curElement, '/');
 if (separator) {
                    numerator = atoi(curElement);
                    denominator = atoi(separator + 1);
 } else {
 double value = atof(curElement);
 ConvertDoubleToURational(value, &numerator, &denominator);
 }
 Put32u(Buffer+(*DataWriteIndex), numerator);
 Put32u(Buffer+(*DataWriteIndex) + 4, denominator);
 (*DataWriteIndex) += 8;
 } else if (format == FMT_SRATIONAL) {
 int numerator;
 int denominator;
 char* separator = strchr(curElement, '/');
 if (separator) {
                    numerator = atoi(curElement);
                    denominator = atoi(separator + 1);
 } else {
 double value = atof(curElement);
 ConvertDoubleToSRational(value, &numerator, &denominator);
 }
 Put32u(Buffer+(*DataWriteIndex), numerator);
 Put32u(Buffer+(*DataWriteIndex) + 4, denominator);
 (*DataWriteIndex) += 8;
 } else if ((components == -1) && ((format == FMT_USHORT) || (format == FMT_SSHORT))) {
                value = atoi(curElement);
 Put16u(Buffer+(*DataWriteIndex), value);
 (*DataWriteIndex) += 4;
 } else {
                value = atoi(curElement);
 Put32u(Buffer+(*DirIndex) + 8, value);  
 }
            curElement = strtok(NULL, ",:");
 }
 if (components == -1) Put32u(componentsPosition, i);  
 }
 (*DirIndex) += 12;
}
