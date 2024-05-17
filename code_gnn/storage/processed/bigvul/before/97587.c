xmlXPathCastStringToBoolean (const xmlChar *val) {
    if ((val == NULL) || (xmlStrlen(val) == 0))
	return(0);
    return(1);
}
