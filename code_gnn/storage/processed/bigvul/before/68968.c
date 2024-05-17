FilterEncodingNode *FLTCreateBinaryCompFilterEncodingNode(void)
{
  FilterEncodingNode *psFilterNode = NULL;

  psFilterNode = FLTCreateFilterEncodingNode();
   
  psFilterNode->pOther = (int *)malloc(sizeof(int));
  (*(int *)(psFilterNode->pOther)) = 0;

  return psFilterNode;
}
