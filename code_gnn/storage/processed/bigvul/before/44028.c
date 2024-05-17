crm_xml_dump(xmlNode * data, int options, char **buffer, int *offset, int *max, int depth)
{
#if 0
    if (is_not_set(options, xml_log_option_filtered)) {
         

        time_t now, next;
        xmlDoc *doc = NULL;
        xmlBuffer *xml_buffer = NULL;

        *buffer = NULL;
        doc = getDocPtr(data);
         
        CRM_CHECK(doc != NULL, return);

        now = time(NULL);
        xml_buffer = xmlBufferCreate();
        CRM_ASSERT(xml_buffer != NULL);

         
        xmlBufferSetAllocationScheme(xml_buffer, XML_BUFFER_ALLOC_DOUBLEIT);

        *max = xmlNodeDump(xml_buffer, doc, data, 0, (options & xml_log_option_formatted));
        if (*max > 0) {
            *buffer = strdup((char *)xml_buffer->content);
        }

        next = time(NULL);
        if ((now + 1) < next) {
            crm_log_xml_trace(data, "Long time");
            crm_err("xmlNodeDump() -> %dbytes took %ds", *max, next - now);
        }

        xmlBufferFree(xml_buffer);
        return;
    }
#endif

    switch(data->type) {
        case XML_ELEMENT_NODE:
             
            dump_xml_element(data, options, buffer, offset, max, depth);
            break;
        case XML_TEXT_NODE:
             
            return;
        case XML_COMMENT_NODE:
            dump_xml_comment(data, options, buffer, offset, max, depth);
            break;
        default:
            crm_warn("Unhandled type: %d", data->type);
            return;

             
    }

}
