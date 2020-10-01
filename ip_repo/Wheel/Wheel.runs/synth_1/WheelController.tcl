# 
# Synthesis run script generated by Vivado
# 

debug::add_scope template.lib 1
set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7a100tcsg324-1

set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir D:/p/system_ability_2018/ip_repo/Wheel/Wheel.cache/wt [current_project]
set_property parent.project_path D:/p/system_ability_2018/ip_repo/Wheel/Wheel.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
read_verilog -library xil_defaultlib {
  D:/p/system_ability_2018/ip_repo/Wheel/Wheel.srcs/sources_1/new/divider.v
  D:/p/system_ability_2018/ip_repo/Wheel/Wheel.srcs/sources_1/new/direction.v
  D:/p/system_ability_2018/ip_repo/Wheel/Wheel.srcs/sources_1/new/pwm.v
  D:/p/system_ability_2018/ip_repo/Wheel/Wheel.srcs/sources_1/new/WheelController.v
}
synth_design -top WheelController -part xc7a100tcsg324-1
write_checkpoint -noxdef WheelController.dcp
catch { report_utilization -file WheelController_utilization_synth.rpt -pb WheelController_utilization_synth.pb }
