.text
    .code 32
    .global vector_start
    .global vector_end

    vector_start:
        ldr r0, =0x10000000
        ldr r1, [r0]
    vector_end:
        .space 1024,0
.end