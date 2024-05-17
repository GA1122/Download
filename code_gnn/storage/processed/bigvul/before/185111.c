         static PassRefPtrWillBeRawPtr<CreateFileResult> create()
//         static CreateFileResult* create()
          {
            return adoptRefWillBeNoop(new CreateFileResult());
//             return new CreateFileResult();
          }