declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) 

define void @dead_memcpy(ptr %src) {
entry:
  %x = alloca [16 x i8]
  call void @llvm.memcpy.p0.p0.i64(ptr %x, ptr %src, i64 16, i1 false)
  ret void
}
