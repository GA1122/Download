xmlXPathCompSwap(xmlXPathStepOpPtr op) {
    int tmp;

#ifndef LIBXML_THREAD_ENABLED
     
    if (xmlXPathDisableOptimizer)
	return;
#endif

    tmp = op->ch1;
    op->ch1 = op->ch2;
    op->ch2 = tmp;
}
