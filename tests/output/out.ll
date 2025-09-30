; ModuleID = 'tests/extensive/memcpy.ll'
source_filename = "tests/extensive/memcpy.ll"

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

define void @memcpy_src_from_alloca(ptr %dst) {
entry:
  %x = alloca [16 x i8], align 1
  call void @llvm.memcpy.p0.p0.i64(ptr %dst, ptr %x, i64 16, i1 false)
  ret void
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
