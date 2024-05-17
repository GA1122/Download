xsltDebugDumpExtensions(FILE * output)
{
    if (output == NULL)
        output = stdout;
    fprintf(output,
            "Registered XSLT Extensions\n--------------------------\n");
    if (!xsltFunctionsHash)
        fprintf(output, "No registered extension functions\n");
    else {
        fprintf(output, "Registered Extension Functions:\n");
        xmlMutexLock(xsltExtMutex);
        xmlHashScanFull(xsltFunctionsHash,
                        (xmlHashScannerFull)
                        xsltDebugDumpExtensionsCallback, output);
        xmlMutexUnlock(xsltExtMutex);
    }
    if (!xsltElementsHash)
        fprintf(output, "\nNo registered extension elements\n");
    else {
        fprintf(output, "\nRegistered Extension Elements:\n");
        xmlMutexLock(xsltExtMutex);
        xmlHashScanFull(xsltElementsHash,
                        (xmlHashScannerFull)
                        xsltDebugDumpExtensionsCallback, output);
        xmlMutexUnlock(xsltExtMutex);
    }
    if (!xsltExtensionsHash)
        fprintf(output, "\nNo registered extension modules\n");
    else {
        fprintf(output, "\nRegistered Extension Modules:\n");
        xmlMutexLock(xsltExtMutex);
        xmlHashScanFull(xsltExtensionsHash,
                        (xmlHashScannerFull)
                        xsltDebugDumpExtModulesCallback, output);
        xmlMutexUnlock(xsltExtMutex);
    }

}
