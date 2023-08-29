cmake_minimum_required(VERSION 3.0.0)
project(kaleidoscope-study VERSION 0.1.0 LANGUAGES C CXX)
set(LLVM_REQUIRED_CXX_STANDARD 17)
include(CTest)
enable_testing()
set(LLVM_DIR /usr/local/lib/cmake/llvm/)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

execute_process(COMMAND llvm-config --libs OUTPUT_VARIABLE LLVM_AVAILABLE_LIBS)
string(STRIP ${LLVM_AVAILABLE_LIBS} LLVM_AVAILABLE_LIBS)
message(STATUS "available libs: ${LLVM_AVAILABLE_LIBS}")
aux_source_directory(src sources)
add_executable(kaleidoscope-study ${sources})
# target_compile_options(kaleidoscope-study PRIVATE -lLLVMWindowsManifest -lLLVMWindowsDriver -lLLVMXRay -lLLVMLibDriver -lLLVMDlltoolDriver -lLLVMCoverage -lLLVMLineEditor -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMX86TargetMCA -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Info -lLLVMWebAssemblyDisassembler -lLLVMWebAssemblyAsmParser -lLLVMWebAssemblyCodeGen -lLLVMWebAssemblyDesc -lLLVMWebAssemblyUtils -lLLVMWebAssemblyInfo -lLLVMVEDisassembler -lLLVMVEAsmParser -lLLVMVECodeGen -lLLVMVEDesc -lLLVMVEInfo -lLLVMSystemZDisassembler -lLLVMSystemZAsmParser -lLLVMSystemZCodeGen -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSparcDisassembler -lLLVMSparcAsmParser -lLLVMSparcCodeGen -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMRISCVDisassembler -lLLVMRISCVAsmParser -lLLVMRISCVCodeGen -lLLVMRISCVDesc -lLLVMRISCVInfo -lLLVMPowerPCDisassembler -lLLVMPowerPCAsmParser -lLLVMPowerPCCodeGen -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMMSP430Disassembler -lLLVMMSP430AsmParser -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMipsDisassembler -lLLVMMipsAsmParser -lLLVMMipsCodeGen -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMLanaiDisassembler -lLLVMLanaiCodeGen -lLLVMLanaiAsmParser -lLLVMLanaiDesc -lLLVMLanaiInfo -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonAsmParser -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMBPFDisassembler -lLLVMBPFAsmParser -lLLVMBPFCodeGen -lLLVMBPFDesc -lLLVMBPFInfo -lLLVMAVRDisassembler -lLLVMAVRAsmParser -lLLVMAVRCodeGen -lLLVMAVRDesc -lLLVMAVRInfo -lLLVMARMDisassembler -lLLVMARMAsmParser -lLLVMARMCodeGen -lLLVMARMDesc -lLLVMARMUtils -lLLVMARMInfo -lLLVMAMDGPUTargetMCA -lLLVMAMDGPUDisassembler -lLLVMAMDGPUAsmParser -lLLVMAMDGPUCodeGen -lLLVMAMDGPUDesc -lLLVMAMDGPUUtils -lLLVMAMDGPUInfo -lLLVMAArch64Disassembler -lLLVMAArch64AsmParser -lLLVMAArch64CodeGen -lLLVMAArch64Desc -lLLVMAArch64Utils -lLLVMAArch64Info -lLLVMOrcJIT -lLLVMMCJIT -lLLVMJITLink -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMOrcTargetProcess -lLLVMOrcShared -lLLVMDWP -lLLVMDebugInfoGSYM -lLLVMOption -lLLVMObjectYAML -lLLVMObjCopy -lLLVMMCA -lLLVMMCDisassembler -lLLVMLTO -lLLVMPasses -lLLVMCFGuard -lLLVMCoroutines -lLLVMObjCARCOpts -lLLVMipo -lLLVMVectorize -lLLVMLinker -lLLVMInstrumentation -lLLVMFrontendOpenMP -lLLVMFrontendOpenACC -lLLVMExtensions -lLLVMDWARFLinker -lLLVMGlobalISel -lLLVMMIRParser -lLLVMAsmPrinter -lLLVMSelectionDAG -lLLVMCodeGen -lLLVMIRReader -lLLVMAsmParser -lLLVMInterfaceStub -lLLVMFileCheck -lLLVMFuzzMutate -lLLVMTarget -lLLVMScalarOpts -lLLVMInstCombine -lLLVMAggressiveInstCombine -lLLVMTransformUtils -lLLVMBitWriter -lLLVMAnalysis -lLLVMProfileData -lLLVMSymbolize -lLLVMDebugInfoPDB -lLLVMDebugInfoMSF -lLLVMDebugInfoDWARF -lLLVMObject -lLLVMTextAPI -lLLVMMCParser -lLLVMMC -lLLVMDebugInfoCodeView -lLLVMBitReader -lLLVMFuzzerCLI -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMTableGen -lLLVMSupport -lLLVMDemangle)

target_link_libraries(kaleidoscope-study 
LLVMWindowsManifest 
LLVMWindowsDriver 
LLVMXRay 
LLVMLibDriver 
LLVMDlltoolDriver 
LLVMCoverage 
LLVMLineEditor 
LLVMXCoreDisassembler 
LLVMXCoreCodeGen 
LLVMXCoreDesc 
LLVMXCoreInfo 
LLVMX86TargetMCA 
LLVMX86Disassembler 
LLVMX86AsmParser 
LLVMX86CodeGen 
LLVMX86Desc 
LLVMX86Info 
LLVMWebAssemblyDisassembler 
LLVMWebAssemblyAsmParser 
LLVMWebAssemblyCodeGen 
LLVMWebAssemblyDesc 
LLVMWebAssemblyUtils 
LLVMWebAssemblyInfo 
LLVMVEDisassembler 
LLVMVEAsmParser 
LLVMVECodeGen 
LLVMVEDesc 
LLVMVEInfo 
LLVMSystemZDisassembler 
LLVMSystemZAsmParser 
LLVMSystemZCodeGen 
LLVMSystemZDesc 
LLVMSystemZInfo 
LLVMSparcDisassembler 
LLVMSparcAsmParser 
LLVMSparcCodeGen 
LLVMSparcDesc 
LLVMSparcInfo 
LLVMRISCVDisassembler 
LLVMRISCVAsmParser 
LLVMRISCVCodeGen 
LLVMRISCVDesc 
LLVMRISCVInfo 
LLVMPowerPCDisassembler 
LLVMPowerPCAsmParser 
LLVMPowerPCCodeGen 
LLVMPowerPCDesc 
LLVMPowerPCInfo 
LLVMNVPTXCodeGen 
LLVMNVPTXDesc 
LLVMNVPTXInfo 
LLVMMSP430Disassembler 
LLVMMSP430AsmParser 
LLVMMSP430CodeGen 
LLVMMSP430Desc 
LLVMMSP430Info 
LLVMMipsDisassembler 
LLVMMipsAsmParser 
LLVMMipsCodeGen 
LLVMMipsDesc 
LLVMMipsInfo 
LLVMLanaiDisassembler 
LLVMLanaiCodeGen 
LLVMLanaiAsmParser 
LLVMLanaiDesc 
LLVMLanaiInfo 
LLVMHexagonDisassembler 
LLVMHexagonCodeGen 
LLVMHexagonAsmParser 
LLVMHexagonDesc 
LLVMHexagonInfo 
LLVMBPFDisassembler 
LLVMBPFAsmParser 
LLVMBPFCodeGen 
LLVMBPFDesc 
LLVMBPFInfo 
LLVMAVRDisassembler 
LLVMAVRAsmParser 
LLVMAVRCodeGen 
LLVMAVRDesc 
LLVMAVRInfo 
LLVMARMDisassembler 
LLVMARMAsmParser 
LLVMARMCodeGen 
LLVMARMDesc 
LLVMARMUtils 
LLVMARMInfo 
LLVMAMDGPUTargetMCA 
LLVMAMDGPUDisassembler 
LLVMAMDGPUAsmParser 
LLVMAMDGPUCodeGen 
LLVMAMDGPUDesc 
LLVMAMDGPUUtils 
LLVMAMDGPUInfo 
LLVMAArch64Disassembler 
LLVMAArch64AsmParser 
LLVMAArch64CodeGen 
LLVMAArch64Desc 
LLVMAArch64Utils 
LLVMAArch64Info 
LLVMOrcJIT 
LLVMMCJIT 
LLVMJITLink 
LLVMInterpreter 
LLVMExecutionEngine 
LLVMRuntimeDyld 
LLVMOrcTargetProcess 
LLVMOrcShared 
LLVMDWP 
LLVMDebugInfoGSYM 
LLVMOption 
LLVMObjectYAML 
LLVMObjCopy 
LLVMMCA 
LLVMMCDisassembler 
LLVMLTO 
LLVMPasses 
LLVMCFGuard 
LLVMCoroutines 
LLVMObjCARCOpts 
LLVMipo 
LLVMVectorize 
LLVMLinker 
LLVMInstrumentation 
LLVMFrontendOpenMP 
LLVMFrontendOpenACC 
LLVMExtensions 
LLVMDWARFLinker 
LLVMGlobalISel 
LLVMMIRParser 
LLVMAsmPrinter 
LLVMSelectionDAG 
LLVMCodeGen 
LLVMIRReader 
LLVMAsmParser 
LLVMInterfaceStub 
LLVMFileCheck 
LLVMFuzzMutate 
LLVMTarget 
LLVMScalarOpts 
LLVMInstCombine 
LLVMAggressiveInstCombine 
LLVMTransformUtils 
LLVMBitWriter 
LLVMAnalysis 
LLVMProfileData 
LLVMSymbolize 
LLVMDebugInfoPDB 
LLVMDebugInfoMSF 
LLVMDebugInfoDWARF 
LLVMObject 
LLVMTextAPI 
LLVMMCParser 
LLVMMC 
LLVMDebugInfoCodeView 
LLVMBitReader 
LLVMFuzzerCLI 
LLVMCore 
LLVMRemarks 
LLVMBitstreamReader 
LLVMBinaryFormat 
LLVMTableGen 
LLVMSupport 
LLVMDemangle)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
