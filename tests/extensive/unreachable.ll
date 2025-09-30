define void @unreachable() {
entry:
  ret void
dead:
  br label %dead2
dead2:
  ret void
}
