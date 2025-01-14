.section ".head.text","ax"
.global _start
.extern _end, kernel_start

_start:
    # code0/code1
    .ascii "MZMZ"
    j entry
    .half 0
    #.word 0

    # text_offset
    .dword 0x200000

    # image_size
    .dword _end - _start

    # flags
    .dword 0

    # version
    .word 2
    # reserved 1
    .word 0
    # reserved 2
    .dword 0

    # magic - yes 0x5643534952 is the same as ASCII string "RISCV"
    .dword 0x5643534952
    # magic 2
    .word 0x05435352

    # reserved 3
    .word 0x40

entry:
    auipc s0, 1040136 # -8440
    call kernel_start
    j 0
    jalr zero, -1904(s0) # 0x10d0

loop:
    j entry

