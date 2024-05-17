crm_xml_init(void)
{
    static bool init = TRUE;

    if(init) {
        init = FALSE;
         
        xmlSetBufferAllocationScheme(XML_BUFFER_ALLOC_DOUBLEIT);

         
        xmlDeregisterNodeDefault(pcmkDeregisterNode);
        xmlRegisterNodeDefault(pcmkRegisterNode);

        __xml_build_schema_list();
    }
}
