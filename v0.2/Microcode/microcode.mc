; Comment   AI is used instead of RI for legacy
o0***, POL MIL, POH MIH, RD II CE, CR  ; NOP
o10***, POL MIL, POH MIH, RD II CE, RD AI CE, CR  ; LD R I
o11***, POL MIL, POH MIH, RD II CE, RD BI CE, RD MIH CE, BO MIL, RD AI, CR  ; LD R MA
o12***, POL MIL, POH MIH, RD II CE, RD BI CE, RD MIH CE, BO MIL, WR AO, CR  ; LD MA R
o20***, POL MIL, POH MIH, RD II CE, RD BI CE, RD PIH, BO PIL, CR  ; JP I
o30***, POL MIL, POH MIH, RD II CE, RD BI CE, EO AI, CR  ; ADD I
o31**+, POL MIL, POH MIH, RD II CE, RD BI CE, EO AI INC, CR  ; ADDC I (carry set)
o31**-, POL MIL, POH MIH, RD II CE, RD BI CE, EO AI, CR  ; ADDC I (carry not set)
o32***, POL MIL, POH MIH, RD II CE, NO BI, EO AI INC, CR  ; INC R
o33***, POL MIL, POH MIH, RD II CE, NO AI, CR  ; ZRO R
o34***, POL MIL, POH MIH, RD II CE, RD BI CE, EOS AI INC, CR  ; SUB I (EOS for not, INC for +1 for 2's comp.)
o35***, POL MIL, POH MIH, RD II CE, RD BI CE, EOS BI INC, CR  ; CMP I
o37***, POL MIL, POH MIH, RD II CE, RD BI CE, BO AI, CR  ; LOGIC I (Used to implement & and |)