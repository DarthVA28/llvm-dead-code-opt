define void @cond_same_target(i1 %c) {
entry:
  br i1 %c, label %t, label %t
t:
  ret void
}
