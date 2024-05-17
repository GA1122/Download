xmlXPathCastNodeSetToBoolean (xmlNodeSetPtr ns) {
    if ((ns == NULL) || (ns->nodeNr == 0))
	return(0);
    return(1);
}
