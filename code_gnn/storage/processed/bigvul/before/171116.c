static void print_data(int fd, const uint8_t *data_ptr, uint32_t tag,
 int type, int count, int indentation) {
 static int values_per_line[NUM_TYPES] = {
 [TYPE_BYTE] = 16,
 [TYPE_INT32] = 4,
 [TYPE_FLOAT] = 8,
 [TYPE_INT64] = 2,
 [TYPE_DOUBLE] = 4,
 [TYPE_RATIONAL] = 2,
 };
 size_t type_size = camera_metadata_type_size[type];
 char value_string_tmp[CAMERA_METADATA_ENUM_STRING_MAX_SIZE];
 uint32_t value;

 int lines = count / values_per_line[type];
 if (count % values_per_line[type] != 0) lines++;

 int index = 0;
 int j, k;
 for (j = 0; j < lines; j++) {
        dprintf(fd, "%*s[", indentation + 4, "");
 for (k = 0;
             k < values_per_line[type] && count > 0;
             k++, count--, index += type_size) {

 switch (type) {
 case TYPE_BYTE:
                    value = *(data_ptr + index);
 if (camera_metadata_enum_snprint(tag,
                                                     value,
                                                     value_string_tmp,
 sizeof(value_string_tmp))
 == OK) {
                        dprintf(fd, "%s ", value_string_tmp);
 } else {
                        dprintf(fd, "%hhu ",
 *(data_ptr + index));
 }
 break;
 case TYPE_INT32:
                    value =
 *(int32_t*)(data_ptr + index);
 if (camera_metadata_enum_snprint(tag,
                                                     value,
                                                     value_string_tmp,
 sizeof(value_string_tmp))
 == OK) {
                        dprintf(fd, "%s ", value_string_tmp);
 } else {
                        dprintf(fd, "%" PRId32 " ",
 *(int32_t*)(data_ptr + index));
 }
 break;
 case TYPE_FLOAT:
                    dprintf(fd, "%0.8f ",
 *(float*)(data_ptr + index));
 break;
 case TYPE_INT64:
                    dprintf(fd, "%" PRId64 " ",
 *(int64_t*)(data_ptr + index));
 break;
 case TYPE_DOUBLE:
                    dprintf(fd, "%0.8f ",
 *(double*)(data_ptr + index));
 break;
 case TYPE_RATIONAL: {
 int32_t numerator = *(int32_t*)(data_ptr + index);
 int32_t denominator = *(int32_t*)(data_ptr + index + 4);
                    dprintf(fd, "(%d / %d) ",
                            numerator, denominator);
 break;
 }
 default:
                    dprintf(fd, "??? ");
 }
 }
        dprintf(fd, "]\n");
 }
}
