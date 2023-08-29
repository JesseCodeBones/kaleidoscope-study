### build script
```bash
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=out/install -DLLVM_TARGETS_TO_BUILD="X86"
```
### llvm targets:
```
AArch64
  AMDGPU
  ARM
  AVR
  BPF
  Hexagon
  Lanai
  Mips
  MSP430
  NVPTX
  PowerPC
  RISCV
  Sparc
  SystemZ
  VE
  WebAssembly
  X86
  XCore
```

### llvm target string parser logic
```
Triple::parseArch
Triple::parseVendor
Triple::parseOS
```
可以解析：  
`auto MyTriple = llvm::Triple("x86-UnknownVendor-linux");`