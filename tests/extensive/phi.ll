define i32 @empty_forwarder(i32 %x, i1 %c) {
entry:
  br i1 %c, label %t, label %f
t:
  br label %mid
f:
  br label %mid
mid:
  %y = phi i32 [ %x, %t ], [ %x, %f ]
  br label %dst
dst:
  ret i32 %y
}