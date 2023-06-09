# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Michael\Dropbox (MIT)\College\Senior Year\6.115 - Microcomputer Project Laboratory\6.115 - Final Project\Software\PSoC Creator\Ball.cydsn\Ball.cyprj
# Date: Mon, 15 May 2023 06:42:12 GMT
#set_units -time ns
create_clock -name {Clock_Millis(routed)} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/dclk_2}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_clock -name {CyBUS_CLK(fixed-function)} -period 41.666666666666664 -waveform {0 20.8333333333333} [get_pins {ClockBlock/clk_bus_glb_ff}]
create_generated_clock -name {Clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 3 5} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 13 25} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_Millis} -source [get_pins {ClockBlock/clk_sync}] -edges {1 24001 48001} [list [get_pins {ClockBlock/dclk_glb_2}]]


# Component constraints for C:\Users\Michael\Dropbox (MIT)\College\Senior Year\6.115 - Microcomputer Project Laboratory\6.115 - Final Project\Software\PSoC Creator\Ball.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Michael\Dropbox (MIT)\College\Senior Year\6.115 - Microcomputer Project Laboratory\6.115 - Final Project\Software\PSoC Creator\Ball.cydsn\Ball.cyprj
# Date: Mon, 15 May 2023 06:41:56 GMT
