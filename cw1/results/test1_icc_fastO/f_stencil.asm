################################################################################
# TEST 1
# 59 lines ASM code
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# mov: 28 counts
# test: 4 counts
# jle:  5 counts (2* 10%, 2* 80%)
# mul:  6 counts
# add:  8 counts
# lea:  4 counts
# memAccess: 0
# QWORD: 23
# DWORD: 0
################################################################################
stencil:
        mov       eax, 1                                        #4.14
        movsxd    rsi, esi                                      #3.1
        movsxd    rdx, ecx                                      #3.1
        test      rsi, rsi                                      #4.28
        jle       ..B1.9        # Prob 10%                      #4.28
        movsd     xmm1, QWORD PTR .L_2il0floatpacket.0[rip]     #6.70
        movsd     xmm0, QWORD PTR .L_2il0floatpacket.1[rip]     #7.70
        mov       QWORD PTR [-40+rsp], r14                      #7.70[spill]
        mov       QWORD PTR [-32+rsp], r15                      #7.70[spill]
        mov       QWORD PTR [-24+rsp], rbx                      #7.70[spill]
        mov       QWORD PTR [-16+rsp], rbp                      #7.70[spill]
..B1.3:                         # Preds ..B1.7 ..B1.2
        mov       r11d, 1                                       #5.16
        lea       r14d, DWORD PTR [rcx+rcx]                     #3.1
        movsxd    r14, r14d                                     #3.1
        mov       ebx, ecx                                      #5.16
        xor       r15d, r15d                                    #7.60
        test      edi, edi                                      #5.30
        jle       ..B1.7        # Prob 10%                      #5.30
        lea       r10, QWORD PTR [r9+rax*8]                     #6.7
        lea       rbp, QWORD PTR [r8+rax*8]                     #6.36
..B1.5:                         # Preds ..B1.5 ..B1.4
        movsxd    rbx, ebx                                      #6.36
        inc       r11d                                          #5.35
        movsd     xmm6, QWORD PTR [rbp+rbx*8]                   #6.36
        mulsd     xmm6, xmm1                                    #6.76
        movsd     QWORD PTR [r10+rbx*8], xmm6                   #6.7
        movsd     xmm2, QWORD PTR [rbp+r15*8]                   #7.36
        add       r15, rdx                                      #5.35
        mulsd     xmm2, xmm0                                    #7.76
        addsd     xmm6, xmm2                                    #7.7
        movsd     QWORD PTR [r10+rbx*8], xmm6                   #7.7
        movsd     xmm3, QWORD PTR [rbp+r14*8]                   #8.36
        add       r14, rdx                                      #5.35
        mulsd     xmm3, xmm0                                    #8.76
        addsd     xmm6, xmm3                                    #8.7
        movsd     QWORD PTR [r10+rbx*8], xmm6                   #8.7
        movsd     xmm4, QWORD PTR [-8+rbp+rbx*8]                #9.36
        mulsd     xmm4, xmm0                                    #9.76
        addsd     xmm6, xmm4                                    #9.7
        movsd     QWORD PTR [r10+rbx*8], xmm6                   #9.7
        movsd     xmm5, QWORD PTR [8+rbp+rbx*8]                 #10.36
        mulsd     xmm5, xmm0                                    #10.76
        addsd     xmm6, xmm5                                    #10.7
        movsd     QWORD PTR [r10+rbx*8], xmm6                   #10.7
        add       ebx, ecx                                      #5.35
        cmp       r11d, edi                                     #5.30
        jle       ..B1.5        # Prob 82%                      #5.30
..B1.7:                         # Preds ..B1.5 ..B1.3
        inc       rax                                           #4.33
        cmp       rax, rsi                                      #4.28
        jle       ..B1.3        # Prob 82%                      #4.28
        mov       r14, QWORD PTR [-40+rsp]                      #[spill]
        mov       r15, QWORD PTR [-32+rsp]                      #[spill]
        mov       rbx, QWORD PTR [-24+rsp]                      #[spill]
        mov       rbp, QWORD PTR [-16+rsp]                      #[spill]
..B1.9:                         # Preds ..B1.8 ..B1.1
        ret                                                     #13.1
