AcpiNsGetInternalNameLength (
    ACPI_NAMESTRING_INFO    *Info)
{
    const char              *NextExternalChar;
    UINT32                  i;


    ACPI_FUNCTION_ENTRY ();


    NextExternalChar = Info->ExternalName;
    Info->NumCarats = 0;
    Info->NumSegments = 0;
    Info->FullyQualified = FALSE;

     
    if (ACPI_IS_ROOT_PREFIX (*NextExternalChar))
    {
        Info->FullyQualified = TRUE;
        NextExternalChar++;

         

        while (ACPI_IS_ROOT_PREFIX (*NextExternalChar))
        {
            NextExternalChar++;
        }
    }
    else
    {
         

        while (ACPI_IS_PARENT_PREFIX (*NextExternalChar))
        {
            Info->NumCarats++;
            NextExternalChar++;
        }
    }

     
    if (*NextExternalChar)
    {
        Info->NumSegments = 1;
        for (i = 0; NextExternalChar[i]; i++)
        {
            if (ACPI_IS_PATH_SEPARATOR (NextExternalChar[i]))
            {
                Info->NumSegments++;
            }
        }
    }

    Info->Length = (ACPI_NAME_SIZE * Info->NumSegments) +
        4 + Info->NumCarats;

    Info->NextExternalChar = NextExternalChar;
}
