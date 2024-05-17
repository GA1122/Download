AcpiNsOpensScope (
    ACPI_OBJECT_TYPE        Type)
{
    ACPI_FUNCTION_ENTRY ();


    if (Type > ACPI_TYPE_LOCAL_MAX)
    {
         

        ACPI_WARNING ((AE_INFO, "Invalid Object Type 0x%X", Type));
        return (ACPI_NS_NORMAL);
    }

    return (((UINT32) AcpiGbl_NsProperties[Type]) & ACPI_NS_NEWSCOPE);
}
