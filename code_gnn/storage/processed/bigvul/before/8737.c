vcard7816_vm_process_apdu(VCard *card, VCardAPDU *apdu,
                          VCardResponse **response)
{
    int bytes_to_copy, next_byte_count, count;
    VCardApplet *current_applet;
    VCardBufferResponse *buffer_response;
    vcard_7816_status_t status;

     
    if (apdu->a_gen_type !=  VCARD_7816_ISO) {
        *response = vcard_make_response(
                        VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
        return VCARD_DONE;
    }

     
    switch (apdu->a_secure_messaging) {
    case 0x0:  
        break;
    case 0x4:  
    case 0x8:  
    case 0xc:  
    default:
         
        *response = vcard_make_response(
                        VCARD7816_STATUS_ERROR_SECURE_NOT_SUPPORTED);
        return VCARD_DONE;
    }

     
    switch (apdu->a_ins) {
    case  VCARD7816_INS_MANAGE_CHANNEL:  
    case  VCARD7816_INS_EXTERNAL_AUTHENTICATE:  
    case  VCARD7816_INS_GET_CHALLENGE:  
    case  VCARD7816_INS_INTERNAL_AUTHENTICATE:  
    case  VCARD7816_INS_ERASE_BINARY:  
    case  VCARD7816_INS_READ_BINARY:  
    case  VCARD7816_INS_WRITE_BINARY:  
    case  VCARD7816_INS_UPDATE_BINARY:  
    case  VCARD7816_INS_READ_RECORD:  
    case  VCARD7816_INS_WRITE_RECORD:  
    case  VCARD7816_INS_UPDATE_RECORD:  
    case  VCARD7816_INS_APPEND_RECORD:  
    case  VCARD7816_INS_ENVELOPE:
    case  VCARD7816_INS_PUT_DATA:
        *response = vcard_make_response(
                            VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
        break;

    case  VCARD7816_INS_SELECT_FILE:
        if (apdu->a_p1 != 0x04) {
            *response = vcard_make_response(
                            VCARD7816_STATUS_ERROR_FUNCTION_NOT_SUPPORTED);
            break;
        }

         
        current_applet = vcard_find_applet(card, apdu->a_body, apdu->a_Lc);
        vcard_select_applet(card, apdu->a_channel, current_applet);
        if (current_applet) {
            unsigned char *aid;
            int aid_len;
            aid = vcard_applet_get_aid(current_applet, &aid_len);
            *response = vcard_response_new(card, aid, aid_len, apdu->a_Le,
                                          VCARD7816_STATUS_SUCCESS);
        } else {
            *response = vcard_make_response(
                             VCARD7816_STATUS_ERROR_FILE_NOT_FOUND);
        }
        break;

    case  VCARD7816_INS_VERIFY:
        if ((apdu->a_p1 != 0x00) || (apdu->a_p2 != 0x00)) {
            *response = vcard_make_response(
                            VCARD7816_STATUS_ERROR_WRONG_PARAMETERS);
        } else {
            if (apdu->a_Lc == 0) {
                 
                count = vcard_emul_get_login_count(card);
                if (count < 0) {
                    *response = vcard_make_response(
                                    VCARD7816_STATUS_ERROR_DATA_NOT_FOUND);
                } else {
                    if (count > 0xf) {
                        count = 0xf;
                    }
                    *response = vcard_response_new_status_bytes(
                                                VCARD7816_SW1_WARNING_CHANGE,
                                                                0xc0 | count);
                    if (*response == NULL) {
                        *response = vcard_make_response(
                                    VCARD7816_STATUS_EXC_ERROR_MEMORY_FAILURE);
                    }
                }
            } else {
                status = vcard_emul_login(card, apdu->a_body, apdu->a_Lc);
                *response = vcard_make_response(status);
            }
        }
        break;

    case VCARD7816_INS_GET_RESPONSE:
        buffer_response = vcard_get_buffer_response(card);
        if (!buffer_response) {
            *response = vcard_make_response(
                            VCARD7816_STATUS_ERROR_DATA_NOT_FOUND);
             
            break;
        }
        bytes_to_copy = MIN(buffer_response->len, apdu->a_Le);
        next_byte_count = MIN(256, buffer_response->len - bytes_to_copy);
        *response = vcard_response_new_bytes(
                        card, buffer_response->current, bytes_to_copy,
                        apdu->a_Le,
                        next_byte_count ?
                        VCARD7816_SW1_RESPONSE_BYTES : VCARD7816_SW1_SUCCESS,
                        next_byte_count);
        buffer_response->current += bytes_to_copy;
        buffer_response->len -= bytes_to_copy;
        if (*response == NULL || (next_byte_count == 0)) {
            vcard_set_buffer_response(card, NULL);
            vcard_buffer_response_delete(buffer_response);
        }
        if (*response == NULL) {
            *response =
                vcard_make_response(VCARD7816_STATUS_EXC_ERROR_MEMORY_FAILURE);
        }
        break;

    case VCARD7816_INS_GET_DATA:
        *response =
            vcard_make_response(VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
        break;

    default:
        *response =
            vcard_make_response(VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
        break;
    }

     
    assert(*response != NULL);
    return VCARD_DONE;
}
