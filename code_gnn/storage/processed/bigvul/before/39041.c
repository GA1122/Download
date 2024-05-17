QTNSort(QTNode *in)
{
	int			i;

	 
	check_stack_depth();

	if (in->valnode->type != QI_OPR)
		return;

	for (i = 0; i < in->nchild; i++)
		QTNSort(in->child[i]);
	if (in->nchild > 1)
		qsort((void *) in->child, in->nchild, sizeof(QTNode *), cmpQTN);
}
