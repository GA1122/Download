void TextCodecUTF8::HandleError(int character,
                                UChar*& destination,
                                bool stop_on_error,
                                bool& saw_error) {
  saw_error = true;
  if (stop_on_error)
    return;
  *destination++ = kReplacementCharacter;
  DCHECK(IsNonCharacter(character));
  int num_bytes_consumed = -character;
  DCHECK_GE(num_bytes_consumed, 1);
  DCHECK_LE(num_bytes_consumed, 3);
  ConsumePartialSequenceBytes(num_bytes_consumed);
}
