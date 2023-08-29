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

### 获取所有libs
`llvm-config --libs`
```
-lLLVMWindowsManifest -lLLVMWindowsDriver -lLLVMXRay -lLLVMLibDriver -lLLVMDlltoolDriver -lLLVMCoverage -lLLVMLineEditor -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMX86TargetMCA -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Info -lLLVMWebAssemblyDisassembler -lLLVMWebAssemblyAsmParser -lLLVMWebAssemblyCodeGen -lLLVMWebAssemblyDesc -lLLVMWebAssemblyUtils -lLLVMWebAssemblyInfo -lLLVMVEDisassembler -lLLVMVEAsmParser -lLLVMVECodeGen -lLLVMVEDesc -lLLVMVEInfo -lLLVMSystemZDisassembler -lLLVMSystemZAsmParser -lLLVMSystemZCodeGen -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSparcDisassembler -lLLVMSparcAsmParser -lLLVMSparcCodeGen -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMRISCVDisassembler -lLLVMRISCVAsmParser -lLLVMRISCVCodeGen -lLLVMRISCVDesc -lLLVMRISCVInfo -lLLVMPowerPCDisassembler -lLLVMPowerPCAsmParser -lLLVMPowerPCCodeGen -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMMSP430Disassembler -lLLVMMSP430AsmParser -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMipsDisassembler -lLLVMMipsAsmParser -lLLVMMipsCodeGen -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMLanaiDisassembler -lLLVMLanaiCodeGen -lLLVMLanaiAsmParser -lLLVMLanaiDesc -lLLVMLanaiInfo -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonAsmParser -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMBPFDisassembler -lLLVMBPFAsmParser -lLLVMBPFCodeGen -lLLVMBPFDesc -lLLVMBPFInfo -lLLVMAVRDisassembler -lLLVMAVRAsmParser -lLLVMAVRCodeGen -lLLVMAVRDesc -lLLVMAVRInfo -lLLVMARMDisassembler -lLLVMARMAsmParser -lLLVMARMCodeGen -lLLVMARMDesc -lLLVMARMUtils -lLLVMARMInfo -lLLVMAMDGPUTargetMCA -lLLVMAMDGPUDisassembler -lLLVMAMDGPUAsmParser -lLLVMAMDGPUCodeGen -lLLVMAMDGPUDesc -lLLVMAMDGPUUtils -lLLVMAMDGPUInfo -lLLVMAArch64Disassembler -lLLVMAArch64AsmParser -lLLVMAArch64CodeGen -lLLVMAArch64Desc -lLLVMAArch64Utils -lLLVMAArch64Info -lLLVMOrcJIT -lLLVMMCJIT -lLLVMJITLink -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMOrcTargetProcess -lLLVMOrcShared -lLLVMDWP -lLLVMDebugInfoGSYM -lLLVMOption -lLLVMObjectYAML -lLLVMObjCopy -lLLVMMCA -lLLVMMCDisassembler -lLLVMLTO -lLLVMPasses -lLLVMCFGuard -lLLVMCoroutines -lLLVMObjCARCOpts -lLLVMipo -lLLVMVectorize -lLLVMLinker -lLLVMInstrumentation -lLLVMFrontendOpenMP -lLLVMFrontendOpenACC -lLLVMExtensions -lLLVMDWARFLinker -lLLVMGlobalISel -lLLVMMIRParser -lLLVMAsmPrinter -lLLVMSelectionDAG -lLLVMCodeGen -lLLVMIRReader -lLLVMAsmParser -lLLVMInterfaceStub -lLLVMFileCheck -lLLVMFuzzMutate -lLLVMTarget -lLLVMScalarOpts -lLLVMInstCombine -lLLVMAggressiveInstCombine -lLLVMTransformUtils -lLLVMBitWriter -lLLVMAnalysis -lLLVMProfileData -lLLVMSymbolize -lLLVMDebugInfoPDB -lLLVMDebugInfoMSF -lLLVMDebugInfoDWARF -lLLVMObject -lLLVMTextAPI -lLLVMMCParser -lLLVMMC -lLLVMDebugInfoCodeView -lLLVMBitReader -lLLVMFuzzerCLI -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMTableGen -lLLVMSupport -lLLVMDemangle
```