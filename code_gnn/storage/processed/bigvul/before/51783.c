dissect_u3v_descriptors(tvbuff_t *tvb, packet_info *pinfo _U_, proto_tree *tree, void *data _U_)
{
    guint8          type;
    gint            offset = 0;
    proto_item *    ti;
    proto_tree *    sub_tree;
    guint32         version;


     
    if (tvb_reported_length(tvb) < 2) {
        return 0;
    }

     
    type = tvb_get_guint8(tvb, 1);

     
    if (type != DESCRIPTOR_TYPE_U3V_INTERFACE) {
        return 0;
    }

    ti = proto_tree_add_item(tree, hf_u3v_device_info_descriptor, tvb, offset, -1, ENC_NA);
    tree = proto_item_add_subtree(ti, ett_u3v_device_info_descriptor);

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_bLength, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    ti = proto_tree_add_item(tree, hf_u3v_device_info_descriptor_bDescriptorType, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    proto_item_append_text(ti, " (U3V INTERFACE)");
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_bDescriptorSubtype, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    if (!tvb_bytes_exist(tvb, offset, 4)) {
         
        return offset;
    }
    version = tvb_get_letohl(tvb, offset);
    ti = proto_tree_add_item(tree, hf_u3v_device_info_descriptor_bGenCPVersion, tvb, offset, 4, ENC_NA);
    proto_item_append_text(ti, ": %u.%u", version >> 16, version & 0xFFFF);
    sub_tree = proto_item_add_subtree(ti, ett_u3v_device_info_descriptor_gencp_version);
    proto_tree_add_item(sub_tree, hf_u3v_device_info_descriptor_bGenCPVersion_minor, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    proto_tree_add_item(sub_tree, hf_u3v_device_info_descriptor_bGenCPVersion_major, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

     
    if (!tvb_bytes_exist(tvb, offset, 4)) {
         
        return offset;
    }
    version = tvb_get_letohl(tvb, offset);
    ti = proto_tree_add_item(tree, hf_u3v_device_info_descriptor_bU3VVersion, tvb, offset, 4, ENC_NA);
    proto_item_append_text(ti, ": %u.%u", version >> 16, version & 0xFFFF);
    sub_tree = proto_item_add_subtree(ti, ett_u3v_device_info_descriptor_u3v_version);
    proto_tree_add_item(sub_tree, hf_u3v_device_info_descriptor_bU3VVersion_minor, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    proto_tree_add_item(sub_tree, hf_u3v_device_info_descriptor_bU3VVersion_major, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iDeviceGUID, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iVendorName, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iModelName, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iFamilyName, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iDeviceVersion, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iManufacturerInfo, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iSerialNumber, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_item(tree, hf_u3v_device_info_descriptor_iUserDefinedName, tvb, offset, 1, ENC_LITTLE_ENDIAN);
    offset++;

     
    proto_tree_add_bitmask(tree, tvb, offset, hf_u3v_device_info_descriptor_bmSpeedSupport,
                           ett_u3v_device_info_descriptor_speed_support, speed_support_fields, ENC_LITTLE_ENDIAN);

    offset++;

    return offset;
}
