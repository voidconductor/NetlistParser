//
// Verilog description for cell VASYA, 
// 10/12/15 14:17:12
//
// Precision RTL Plus , 2014b.10_32-bit//


module VASYA ( clk, rst, result ) ;

    input clk ;
    input rst ;
    output [3:0]result ;

    wire [3:0]result_dup_0;
    wire clk_int, rst_int, nx13771z1, nx12774z1, nx11777z1, nx10780z1, nx10780z2
         ;



    OUTBUF result_obuf_0_ (.PAD (result[0]), .D (result_dup_0[0])) ;
    OUTBUF result_obuf_1_ (.PAD (result[1]), .D (result_dup_0[1])) ;
    OUTBUF result_obuf_2_ (.PAD (result[2]), .D (result_dup_0[2])) ;
    OUTBUF result_obuf_3_ (.PAD (result[3]), .D (result_dup_0[3])) ;
    INBUF rst_ibuf (.Y (rst_int), .PAD (rst)) ;
    INBUF clk_ibuf (.Y (clk_int), .PAD (clk)) ;
    INV modgen_counter_result_ix13771z26622 (.Y (nx13771z1), .A (result_dup_0[0]
        )) ;
    AX1C modgen_counter_result_ix11777z8206 (.Y (nx11777z1), .A (result_dup_0[1]
         ), .B (result_dup_0[0]), .C (result_dup_0[2])) ;
    AX1 modgen_counter_result_ix10780z14339 (.Y (nx10780z1), .A (nx10780z2), .B (
        result_dup_0[2]), .C (result_dup_0[3])) ;
    NAND2 NOT_modgen_counter_result_inc_ix10780z24339 (.Y (nx10780z2), .A (
          result_dup_0[1]), .B (result_dup_0[0])) ;
    XOR2 ix12774z10876 (.Y (nx12774z1), .A (result_dup_0[0]), .B (
         result_dup_0[1])) ;
    DFN1C0 modgen_counter_result_reg_q_3_ (.Q (result_dup_0[3]), .D (nx10780z1)
           , .CLK (clk_int), .CLR (rst_int)) ;
    DFN1C0 modgen_counter_result_reg_q_2_ (.Q (result_dup_0[2]), .D (nx11777z1)
           , .CLK (clk_int), .CLR (rst_int)) ;
    DFN1C0 modgen_counter_result_reg_q_1_ (.Q (result_dup_0[1]), .D (nx12774z1)
           , .CLK (clk_int), .CLR (rst_int)) ;
    DFN1C0 modgen_counter_result_reg_q_0_ (.Q (result_dup_0[0]), .D (nx13771z1)
           , .CLK (clk_int), .CLR (rst_int)) ;
endmodule

