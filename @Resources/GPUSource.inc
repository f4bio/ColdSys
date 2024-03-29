; TenSystem for Rainmeter - GPU Widget
; Common back-end code
; Version 4.1.3

[Rainmeter]
 ; This is used to show the Task Manager button only when the mouse is hovering over the widget.
 MouseOverAction=[!ShowMeter LaunchTaskManager][!Redraw]
 MouseLeaveAction=[!HideMeter LaunchTaskManager][!Redraw]
 AccurateText=1

[mGPUTemp]
Measure=Plugin
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=GpuAmd
SensorType=Temperature

[MeasureGPU]
 ; This measures the usage of the GPU.
 ; As Rainmeter doesn't have a native GPU measure, 
 ; the UsageMonitor plugin is used for this purpose to obtain the values from Performance Monitor (PerfMon)
 ; Index is set to 0 to return total of GPU usage across all processes.
 ; This requires Windows 10 Fall Creators Update (version 1709) or later to work.
Measure=Plugin
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=GpuAmd
SensorType=Load
Alias=GPU
Index=0

[MeasureDedicatedVRAM]
 ; This measure retrieves the amount of dedicated GPU memory in use.
 ; Again, the UsageMonitor plugin is used.
 ; Alias=VRAM returns the total amount of both dedicated and shared VRAM in use, 
 ; which is not what we want.
 ; Therefore, we manually define the Category (referred to as Object in PerfMon) and Counter we need,
 ; which in this case is GPU Adapter Memory and Dedicated Usage, respectively.
 Measure=Plugin
 Plugin=UsageMonitor
 Category=GPU Adapter Memory
 Counter=Dedicated Usage
 Index=0
 
[CalcDedicatedVRAM]
 ; We use a Calc measure here to retrieve the numerical value returned from MeasureDedicatedVRAM,
 ; as the measure above returns both a string and numerical value,
 ; with the string value being the name of the instance in PerfMon instead of the numerical value.
 ; If we simply passed MeasureDedicatedVRAM into the String meter for the tooltip below, 
 ; it will take the string value instead of the numerical value, 
 ; and cause it to display 0 instead.
 ; We will also use this Calc measure to convert the amount of VRAM from bytes to gigabytes,
 ; by dividing the value we get by 1073741824.
 Measure=Calc
 ; Be sure to add the colon at the end of the measure name as a section variable,
 ; to ensure that we are retrieving the numerical value.
 Formula=[MeasureDedicatedVRAM:]/1073741824
 DynamicVariables=1
 
[MeasureSharedVRAM]
 ; This measure retrieves the amount of shared GPU memory in use.
 ; Same with the measure used to retrieve the amount of dedicated GPU memory in use,
 ; but with the counter defined as Shared Usage instead.
 Measure=Plugin
 Plugin=UsageMonitor
 Category=GPU Adapter Memory
 Counter=Shared Usage
 Index=0
 
[CalcSharedVRAM]
 ; Same thing here as with the measure for dedicated VRAM (MeasureDedicatedVRAM).
 Measure=Calc
 ; Be sure to add the colon at the end of the measure name as a section variable,
 ; to ensure that we are retrieving the numerical value.
 Formula=[MeasureSharedVRAM:]/1073741824
 DynamicVariables=1

[BG]
 ; This defines the size of the skin itself.
 ; The colour is defined in the variant skins themselves.
 Meter=Shape
 Shape=Rectangle 0,0,200,50 | StrokeWidth 0 | Stroke Color 0,0,0,0 | Extend BackgroundColor
 BackgroundColor=Fill Color #BGColor#

[Tooltip]
 ; The tooltip displays the following info:
 ; - GPU processor usage, as a percentage
 ; - dedicated VRAM usage
 ; - shared VRAM usage
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
 TooltipTitle="GPU Usage"
 TooltipText="Processor Usage: [MeasureGPU:0]%#CRLF#Dedicated GPU Memory Usage: [CalcDedicatedVRAM:1] GB#CRLF#Shared GPU Memory Usage: [CalcSharedVRAM:1] GB"
 
; The graph consists of two parts: a Line meter and a Histogram meter.
; The Line meter is used to visually display the peaks of the graphs,
; while the Histogram meter is used to "fill in" the graph beneath the Line meter.
; The graph's colours are set in the variant skins themselves.
[GPUGraphMax]
 Meter=Line
 MeasureName=MeasureGPU
 LineColor=#GraphLineColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[GPUGraph]
 Meter=Histogram
 MeasureName=MeasureGPU
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
 MeasureName=mGPUTemp
 Postfix=" [\x00B0]C"
 ;Text="GPU"
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