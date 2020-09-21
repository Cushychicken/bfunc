module phase_acc(
    clock     ,
    reset     ,
    freq0_set ,
    freq1_set ,
    freq_sel  ,
    phase0_set,
    phase1_set,
    phase_sel ,
    phase_out    
);

// Input Ports
input clock;
input reset;

input freq_sel;
input [27:0] freq0_set;
input [27:0] freq1_set;

input phase_sel;
input [11:0] phase0_set;
input [11:0] phase1_set;

// Output Ports
output reg [11:0] phase_out;

// Internal Registers
reg [27:0] phase_acc;

// Internal Buses
wire [11:0] phase_off;

// Internal Bus Assignments
assign phase_off = (phase_sel) ? phase1_set:phase0_set; 

// Logic Begin
always @(posedge reset or posedge clock) begin
    if (reset) begin
        phase_acc <= 28'h000_0000;
    end 
    else begin
        if (freq_sel) begin
            phase_acc <= phase_acc + freq1_set;
        end else 
            phase_acc <= phase_acc + freq0_set;
        end

        phase_out <= phase_acc[27:16] + phase_off;  
    end
// End always @() block

endmodule
