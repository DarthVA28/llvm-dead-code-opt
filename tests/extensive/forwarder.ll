define i32 @forwarder(i32 %x) {
entry:
  br label %mid
mid:
  %z = add i32 %x, 1       
  br label %dst
dst:
  ret i32 %z
}
