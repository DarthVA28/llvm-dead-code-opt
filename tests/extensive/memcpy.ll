declare void @llvm.memcpy.p0.p0.i64(ptr nocapture, ptr nocapture, i64, i1)

define void @memcpy_src_from_alloca(ptr %dst) {
entry:
  %x = alloca [16 x i8]
  call void @llvm.memcpy.p0.p0.i64(ptr %dst, ptr %x, i64 16, i1 0)
  ret void
}