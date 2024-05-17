vcard7816_file_system_process_apdu(VCard *card, VCardAPDU *apdu,
                                   VCardResponse **response)
{
     
    *response = vcard_make_response(
                    VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
    return VCARD_DONE;
}
