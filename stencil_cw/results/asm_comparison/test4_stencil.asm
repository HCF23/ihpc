stencil:
        movzx     esi, sil                                      #62.1
        movzx     r11d, dil                                     #62.1
        mov       edi, 1                                        #63.18
        movzx     r10d, cl                                      #62.1
        test      esi, esi                                      #63.32
        jle       ..B2.9        # Prob 10%                      #63.32
        mov       ecx, r10d                                     #68.60
        movsd     xmm0, QWORD PTR .L_2il0floatpacket.1[rip]     #67.70
        neg       ecx                                           #68.60
        mov       QWORD PTR [-72+rsp], r12                      #68.60[spill]
        mov       QWORD PTR [-64+rsp], r13                      #68.60[spill]
        mov       QWORD PTR [-56+rsp], r14                      #68.60[spill]
        mov       QWORD PTR [-48+rsp], r15                      #68.60[spill]
        mov       QWORD PTR [-40+rsp], rbx                      #68.60[spill]
        mov       QWORD PTR [-32+rsp], rbp                      #68.60[spill]
..B2.3:                         # Preds ..B2.7 ..B2.2
        mov       r13d, 1                                       #65.20
        test      r11d, r11d                                    #65.34
        jle       ..B2.7        # Prob 10%                      #65.34
        mov       DWORD PTR [-16+rsp], esi                      #69.60[spill]
        lea       ebx, DWORD PTR [rdi+rcx]                      #68.60
        mov       QWORD PTR [-24+rsp], r9                       #69.60[spill]
        lea       r14d, DWORD PTR [rdi+r10]                     #69.60
        lea       r12, QWORD PTR [r8+rdi*2]                     #67.36
        lea       rbp, QWORD PTR [r9+rdi*2]                     #67.7
..B2.5:                         # Preds ..B2.5 ..B2.4
        mov       r15d, r10d                                    #67.60
        pxor      xmm1, xmm1                                    #67.36
        imul      r15d, r13d                                    #67.60
        inc       r13d                                          #65.39
        mov       esi, r15d                                     #67.36
        movzx     r9d, WORD PTR [r12+rsi*2]                     #67.36
        lea       eax, DWORD PTR [rbx+r15]                      #68.60
        cvtsi2sd  xmm1, r9d                                     #67.36
        mulsd     xmm1, xmm0                                    #67.76
        add       r15d, r14d                                    #69.60
        cvttsd2si r9d, xmm1                                     #67.76
        movsxd    rax, eax                                      #68.36
        mov       WORD PTR [rbp+rsi*2], r9w                     #67.7
        movzx     r13d, r13b                                    #65.39
        movzx     edx, WORD PTR [r8+rax*2]                      #68.36
        mov       eax, 1717986919                               #68.71
        imul      edx                                           #68.71
        mov       eax, 1717986919                               #69.71
        sar       edx, 2                                        #68.71
        add       r9d, edx                                      #68.7
        mov       WORD PTR [rbp+rsi*2], r9w                     #68.7
        movzx     r15d, WORD PTR [r8+r15*2]                     #69.36
        imul      r15d                                          #69.71
        mov       eax, 1717986919                               #70.71
        sar       edx, 2                                        #69.71
        add       r9d, edx                                      #69.7
        mov       WORD PTR [rbp+rsi*2], r9w                     #69.7
        movzx     r15d, WORD PTR [-2+r12+rsi*2]                 #70.36
        imul      r15d                                          #70.71
        mov       eax, 1717986919                               #71.71
        sar       edx, 2                                        #70.71
        add       r9d, edx                                      #70.7
        mov       WORD PTR [rbp+rsi*2], r9w                     #70.7
        movzx     r15d, WORD PTR [2+r12+rsi*2]                  #71.36
        imul      r15d                                          #71.71
        sar       edx, 2                                        #71.71
        add       r9d, edx                                      #71.7
        mov       WORD PTR [rbp+rsi*2], r9w                     #71.7
        cmp       r13d, r11d                                    #65.34
        jle       ..B2.5        # Prob 82%                      #65.34
        mov       esi, DWORD PTR [-16+rsp]                      #[spill]
        mov       r9, QWORD PTR [-24+rsp]                       #[spill]
..B2.7:                         # Preds ..B2.6 ..B2.3
        inc       edi                                           #63.37
        movzx     edi, dil                                      #63.37
        cmp       edi, esi                                      #63.32
        jle       ..B2.3        # Prob 82%                      #63.32
        mov       r12, QWORD PTR [-72+rsp]                      #[spill]
        mov       r13, QWORD PTR [-64+rsp]                      #[spill]
        mov       r14, QWORD PTR [-56+rsp]                      #[spill]
        mov       r15, QWORD PTR [-48+rsp]                      #[spill]
        mov       rbx, QWORD PTR [-40+rsp]                      #[spill]
        mov       rbp, QWORD PTR [-32+rsp]                      #[spill]
..B2.9:                         # Preds ..B2.8 ..B2.1
        ret                                                     #74.1
