MagickPrivate FxInfo *AcquireFxInfo(const Image *image,const char *expression,
  ExceptionInfo *exception)
{
  char
    fx_op[2];

  const Image
    *next;

  FxInfo
    *fx_info;

  register ssize_t
    i;

  fx_info=(FxInfo *) AcquireMagickMemory(sizeof(*fx_info));
  if (fx_info == (FxInfo *) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  (void) ResetMagickMemory(fx_info,0,sizeof(*fx_info));
  fx_info->exception=AcquireExceptionInfo();
  fx_info->images=image;
  fx_info->colors=NewSplayTree(CompareSplayTreeString,RelinquishMagickMemory,
    RelinquishAlignedMemory);
  fx_info->symbols=NewSplayTree(CompareSplayTreeString,RelinquishMagickMemory,
    RelinquishMagickMemory);
  fx_info->view=(CacheView **) AcquireQuantumMemory(GetImageListLength(
    fx_info->images),sizeof(*fx_info->view));
  if (fx_info->view == (CacheView **) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  i=0;
  next=GetFirstImageInList(fx_info->images);
  for ( ; next != (Image *) NULL; next=next->next)
  {
    fx_info->view[i]=AcquireVirtualCacheView(next,exception);
    i++;
  }
  fx_info->random_info=AcquireRandomInfo();
  fx_info->expression=ConstantString(expression);
  fx_info->file=stderr;
  (void) SubstituteString(&fx_info->expression," ","");   
   
  (void) SubstituteString(&fx_info->expression,"-","-1.0*");
  (void) SubstituteString(&fx_info->expression,"^-1.0*","^-");
  (void) SubstituteString(&fx_info->expression,"E-1.0*","E-");
  (void) SubstituteString(&fx_info->expression,"e-1.0*","e-");
   
  fx_op[1]='\0';
  *fx_op=(char) LeftShiftOperator;
  (void) SubstituteString(&fx_info->expression,"<<",fx_op);
  *fx_op=(char) RightShiftOperator;
  (void) SubstituteString(&fx_info->expression,">>",fx_op);
  *fx_op=(char) LessThanEqualOperator;
  (void) SubstituteString(&fx_info->expression,"<=",fx_op);
  *fx_op=(char) GreaterThanEqualOperator;
  (void) SubstituteString(&fx_info->expression,">=",fx_op);
  *fx_op=(char) EqualOperator;
  (void) SubstituteString(&fx_info->expression,"==",fx_op);
  *fx_op=(char) NotEqualOperator;
  (void) SubstituteString(&fx_info->expression,"!=",fx_op);
  *fx_op=(char) LogicalAndOperator;
  (void) SubstituteString(&fx_info->expression,"&&",fx_op);
  *fx_op=(char) LogicalOrOperator;
  (void) SubstituteString(&fx_info->expression,"||",fx_op);
  *fx_op=(char) ExponentialNotation;
  (void) SubstituteString(&fx_info->expression,"**",fx_op);
  return(fx_info);
}