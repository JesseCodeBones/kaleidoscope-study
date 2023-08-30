## SSA & phi
所有 LLVM 指令都使用 SSA (Static Single Assignment，静态一次性赋值) 方式表示  
```
a = 1;
if (v < 10)
    a = 2;
b = a;
```

在LLVM中会被lr解释成

```
entryBasicBlock:
a1 = 1;
if (v < 10)
    ifTrueBasicBlock: 
    a2 = 2;
endBasicBlock:
b = PHI(a1 entryBasicBlock, a2 ifTrueBasicBlock);
```
这里的b是明写的控制流选择复制。  
当endBasicBlock的控制流是从entryBasicBlock来的，就使用a1的值。
如果是从ifTrueBasicBlock来的，就是用a2的值。  


