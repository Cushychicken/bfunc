`timescale 1ns / 1ns

`include "source/phase_acc.v"

module test_phase_acc;
    
    // DUT Input Signals
    reg reset, clock, freq_sel, phase_sel;
    reg [27:0] freq0_set, freq1_set;
    reg [11:0] phase0_set, phase1_set;

    // DUT Output Signals
    wire [11:0] phase_out;

    // Filehandle for output data
    integer testfile;
    integer start_save = 0;

    // Clock Generation (1GHz clock in simulation)
    always 
        #1 clock = !clock;

    // Initialize Clock/Reset, and Module Inputs
    initial begin
        testfile = $fopen("tmp/testdata.txt", "w");
        clock      = 0;
        reset      = 1;
        
        freq_sel   = 0;
        phase_sel  = 0;
        
        freq0_set = 28'h000_A7C6; // FTW for ~10kHz output
        freq1_set = 28'h001_4F8C; // FTW for ~20kHz output
        phase0_set = 12'h000; // Phase for 0deg output offset
        phase1_set = 12'h3FF; // Phase for 90deg output offset

        // Holds Reset for two cycles, then releases
        @(posedge clock);
        @(posedge clock);
        reset      = 0;
    end

    // Monitor setup / Test Routine
    initial begin
        $monitor("Starting testbench...");
        $monitor("time,freq0_set,freq1_set,freq_sel,phase0_set,phase1_set,phase_sel,phase_out");
        $fwrite(testfile, "time,freq0_set,freq1_set,freq_sel,phase0_set,phase1_set,phase_sel,phase_out\n");
        $dumpfile("tmp/rawwaves_testrun.vcd");
        $dumpvars(0);
        @(negedge reset);
        start_save = 1;
        @(posedge clock);

        #10 reset = 0;

        #1000
        
        freq_sel   = 1;

        #1000

        freq_sel   = 0;
        phase_sel  = 1;

        #10000
        $fclose(testfile);
        $finish;
    end 

    // Writes all signals to files for postprocessing
    always @(posedge clock) begin
        if (start_save) begin
            $fwrite(testfile, "%0d,", $time);
            $fwrite(testfile, "%0d,", freq0_set);
            $fwrite(testfile, "%0d,", freq1_set);
            $fwrite(testfile, "%0d,", freq_sel);
            $fwrite(testfile, "%0d,", phase0_set);
            $fwrite(testfile, "%0d,", phase1_set);
            $fwrite(testfile, "%0d,", phase_sel);
            $fwrite(testfile, "%0d\n", phase_out);
        end
    end

    // DUT Instance
    phase_acc phase_acc0(
        .clock       (clock),
        .reset       (reset),
        .freq0_set   (freq0_set),
        .freq1_set   (freq1_set),
        .freq_sel    (freq_sel),
        .phase0_set  (phase0_set),
        .phase1_set  (phase1_set),
        .phase_sel   (phase_sel),
        .phase_out   (phase_out)
    );

endmodule
