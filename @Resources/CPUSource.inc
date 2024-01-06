; TenSystem for Rainmeter - CPU Widget
; Common back-end code
; Version 4.1.3

[Rainmeter]
; This is used to show the Task Manager button only when the mouse is hovering over the widget.
MouseOverAction=[!ShowMeter LaunchTaskManager][!Redraw]
MouseLeaveAction=[!HideMeter LaunchTaskManager][!Redraw]
AccurateText=1

; Previously, the CPU widget would only retireve CPU usage info from only the first physical CPU if used on a device or system with two physical CPUs installed,
; by defining the Name (instance) as 0,_Total (total of first physical CPU) instead of _Total.
; This was due to how the base/max CPU frequency (the MeasureCPUFrequencyMax measure) was obtained (by reading the reported value of the first core of the first physical processor from the Registry).
; However, with the change to how this value is obtained (through the Processor Frequency counter from Performance Monitor (PerfMon)),
; this is now no longer necessary as the information sources have all been aligned,
; with PerfMon's Processor Information object capable of retrieving the overall/total processor frequency from both physical CPUs (if they are there).
; Therefore, the Name (instance) have now all been changed to _Total instead of 0,_Total as it was previously.

[mCPUTemp]
Measure=Plugin  
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=Cpu
SensorType=Temperature

[MeasureCPU]
 ; To maintain consistency with the way we obtain the CPU's current frequency, 
 ; we use the UsageMonitor plugin instead of Rainmeter's built-in CPU measure to obtain the usage percentage of the CPU as well here.
 ; Unfortunately, Rainmeter's built-in CPU measure doesn't measure the frequency of the CPU (yet), 
 ; so we will have to use UsageMonitor to obtain the values from Performance Monitor (PerfMon) instead.
Measure=Plugin
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=Cpu
SensorType=Load
Category=Processor Information
Counter=% Processor Time
Name=_Total
; We need to explicitly set MaxValue as 100 for the graph to scale properly.
MaxValue=100
 
[MeasureCPUFrequencyPercent]
 ; Because of how Intel's TurboBoost/SpeedStep/SpeedShift technology works,
 ; PerfMon's Processor Frequency counter does not accurately reflect the actual current speed of the processor.
 ; To get the actual current speed of the processor, 
 ; we will instead have to obtain it via some calculations done with "% Processor Performance" counter.
 ; We are not using "% of Maximum Frequency" as its description in PerfMon points out that,
 ; as some processors are capable of regulating their frequency outside of Windows' control,
 ; "% of Maximum Frequency" may return inaccurate values, and suggests using "% Processor Performance" instead,
 ; which will be what we will be doing here.
Measure=Plugin
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=Cpu
SensorType=Frequency
Category=Processor Information
Counter=% Processor Performance
Name=_Total

[MeasureCPUFrequencyMax]
 ; We now retrieve the maximum/base speed of the CPU.
 ; The Processor Frequency counter is an (apparently) static counter that reports the current rated base/max speed of the CPU(s) installed.
 ; This value should be static and not be a dynamically-changing value,
 ; but we avoid setting UpdateDivider=-1 on this measure just in case.
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=Cpu
SensorType=Frequency
SensorValueName=Max
Category=Processor Information
Counter=Processor Frequency
Name=_Total

[CalcActualCPUFrequency]
 ; We now calculate the actual current speed of the CPU's frequency.
Measure=Calc
 ; Formula: ([MeasureCPUFrequencyPercent] ÷ 100) × [MeasureCPUFrequencyMax]
 ; Be sure to add the colon at the end of the measure name as a section variable,
 ; to ensure that we are retrieving the numerical value.
Formula=([MeasureCPUFrequencyPercent:]/100)*[MeasureCPUFrequencyMax:]
DynamicVariables=1

[BG]
 ; This defines the size of the skin itself.
 ; The colour is defined in the variant skins themselves.
Meter=Shape
Shape=Rectangle 0,0,200,50 | StrokeWidth 0 | Stroke Color 0,0,0,0 | Extend BackgroundColor
BackgroundColor=Fill Color #BGColor#
 
[Tooltip]
 ; The tooltip displays the following info:
 ; - CPU processor usage, as a percentage
 ; - the current frequency, or speed, of the processor, recalculated here into GHz
 ; - the maximum (base) frequency, or speed, of the processor, recalculated here into GHz
 ; The text meter used here is essentially made invisible but not hidden,
 ; as that would cause the tooltip to not show.
 Meter=STRING
 X=0
 Y=0
 W=200
 H=50
 FontColor=0,0,0,0
 FontSize=0
 DynamicVariables=1
 Text=""
 TooltipTitle="CPU Usage"
 TooltipText="Processor Usage: [MeasureCPU:0]%#CRLF#Speed: [CalcActualCPUFrequency:/1000,2] GHz#CRLF#Base Speed: [MeasureCPUFrequencyMax:/1000,2] GHz"
 
; The graph consists of two parts: a Line meter and a Histogram meter.
; The Line meter is used to visually display the peaks of the graphs,
; while the Histogram meter is used to "fill in" the graph beneath the Line meter.
; The graph's colours are set in the variant skins themselves.
[CPUGraphMax]
 Meter=Line
 MeasureName=MeasureCPU
 LineColor=#GraphLineColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[CPUGraph]
 Meter=Histogram
 MeasureName=MeasureCPU
 PrimaryColor=#GraphFillColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[Label]
 ; This is the background text on the widget.
 ; The font colour is set in the variant skins themselves.
 Meter=STRING
 X=190
 Y=7
 FontColor=#LabelColor#
 FontFace=Segoe UI Light
 FontSize=20
 StringAlign=Right
 StringStyle=NORMAL
 AntiAlias=1
 MeasureName=mCPUTemp
 Postfix=" [\x00B0]C"
 ;Text="CPU"

[ToolbarButtonBG]
 ; This is the background that appears when hovering over each of the toolbar's buttons.
 ; The position of the toolbar button background is dynamically set when hovering over each button.
 ; Dynamically repositioning a single shape meter on hover instead of having a separate hover button background for each button means less repetitive Meters need to be used,
 ; allowing one Shape meter to suffice for all the toolbar buttons instead of each requiring their own Meter.
 Meter=Shape
 Shape=Rectangle 0,0,24,24 | FillColor #ToolbarButtonHoverColor#,60 | StrokeWidth 1 | StrokeColor #ToolbarButtonHoverColor#,192
 Hidden=1

[LaunchTaskManager]
 ; This String meter displays a glyph from the Segoe MDL2 Assets font and acts as a link to launch Task Manager.
 Meter=String
 ; Segoe MDL2 Assets character:
 ; Glyph name: Diagnostic
 ; Glyph code: U+E9D9
 Text=[\xE9D9]
 FontColor=#ToolbarButtonGlyphColor#
 FontFace=Segoe MDL2 Assets 
 FontSize=12
 StringStyle=Normal
 Padding=4,4,-4,-4
 AntiAlias=1
 X=175
 Y=1
 W=24
 H=24
 Hidden=1
 TooltipText="Open Task Manager"
 ; Hide the informational tooltip when the mouse is hovering over this meter to ensure that only one tooltip is being displayed at any one time,
 ; as well as giving it a background colour to make it look like a button.
 MouseOverAction=[!SetOption ToolbarButtonBG X [#CURRENTSECTION#:X]][!SetOption ToolbarButtonBG Y [#CURRENTSECTION#:Y]][!UpdateMeter ToolbarButtonBG][!ShowMeter ToolbarButtonBG][!HideMeter Tooltip][!Redraw]
 MouseLeaveAction=[!HideMeter ToolbarButtonBG][!ShowMeter Tooltip][!Redraw]
 LeftMouseUpAction=["%windir%\system32\taskmgr.exe"]