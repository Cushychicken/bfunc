`timescale 1ns / 1ns

`include "source/phase_acc.v"

module test_phase_acc;

    reg reset, clock;
    reg [27:0] nco_set;

    wire [11:0] phase_out;

    initial begin
        $monitor("Starting testbench...");
        $dumpfile("tmp/rawwaves_testrun.vcd");
        $dumpvars(0);
        clock  = 0;
        reset  = 0;
        nco_set = 28'h100;

        #5  reset = 1;
        #10 reset = 0;

        #1000

        nco_set = 28'hA7C5_AC47; // FTW for ~10kHz output

        #1000
        
        nco_set = 28'h0004_4B83; // FTW for ~1Hz output

        #1000

        $finish;
    end 

    always 
        #1 clock = !clock;

    phase_acc phase_acc0(
        .reset       (reset),
        .clock       (clock),
        .nco_set     (nco_set),
        .phase_out   (phase_out)
    );

endmodule
