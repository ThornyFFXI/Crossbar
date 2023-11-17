# WARNING - Deprecated
While this will remain available for users of Ashita pre-4.15, or those who are curious, it has been deprecated.  Users of current Ashita should look at https://github.com/ThornyFFXI/tCrossBar instead.

# Crossbar
Crossbar is a plugin based on the XIVCrossbar addon by Aliekber(and thus, based on FFXIV's crossbar).  It provides the ability to trigger macros with a controller effectively.  Crossbar handles all of the controller hooks internally and should work with directinput and xinput controllers with no special setup.

# Commands
Crossbar has no commands.  To reload settings, simply reload the plugins.

# Configuration
Upon first load, Crossbar will create a settings file at Ashita/config/crossbar/Charname_CharId/settings.xml.  This allows you to customize UI elements for a personalized experience.  You may want to change theme to dinput.xml if you use a playstation style controller, or compactxinput.xml or compactdinput.xml if you are short on screen real estate.  The priority and double-tap settings allow additional bindings.  If you typically use the right button on your gamepad for confirm and the bottom for cancel, as was the case on most playstation games of prior generations, set 'useplaystationconfirm' to true. Most of it should be self explanatory, and many users may not need to adjust settings at all.

# Binding Macros
Hold L2+R2+L1+R1 to open the binding menu.  From here, you can follow the onscreen prompts to assign keys using your controller.  The binding menu will close automatically if you continue to press your exit key past the main menu, or if you hold L2+R2+L1+R1 for a period of time.  The default period is 1.2 seconds, but this can also be adjusted in configuration under 'menuduration'.  While in the binding menu, the palette you are currently binding to will always be shown regardless of trigger state.  While in the palette editor, the palette you are editing will always be shown regardless of current palette(triggers will also work to display different portions of it).

# Controller Hook
Anytime you have the binding menu open, L2 pressed, or R2 pressed, the controller will not pass dpad, the 4 face buttons, L1, or R1 to the game.  You can bind them as usual for your ingame settings, but be aware they will not be sent under those conditions.  Crossbar will ALWAYS block R2 and L2 being sent to the game while loaded, so you cannot use them for anything ingame.

# Macros / Palettes
Crossbar stores your commands in palettes, each of which contains 24-48 macros.  Each palette contains 3-6 trigger configurations, each of which contains 8 buttons.  To execute a macro, perform the correct trigger configuration then press the corresponding dpad direction or face button(4 each).  The possible trigger configurations are:
* Left Trigger (Simply hold left trigger and press button)
* Right Trigger (Simply hold left trigger and press button)
* Both Triggers, Left depressed first (Press and hold left trigger, then press and hold right trigger, then press button with both depressed.)
* Both Triggers, Right depressed first (Press and hold right trigger, then press and hold left trigger, then press button with both depressed.)
* Left Trigger Double Tap (Press and release left trigger, then quickly re-press and hold left trigger)
* Right Trigger Double Tap (Press and release right trigger, then quickly re-press and hold right trigger)

Please note that you will need to alter settings to enable the latter 3 options.  If you do not enable 'allowpriority', then holding left+right trigger will always correspond to the **Both triggers, Left depressed first** bindings regardless of which order you pressed them.  If you do not enable 'allowdoubletap', then double tapping triggers will not change your macro set and you'll simply be on **Left Trigger** or **Right Trigger**.  Thus, a palette with default settings will only have 24 triggerable macros while a palette with everything enabled will have the full 48.

# Macro Storage
Crossbar automatically creates a new set of palettes and XML for each job you change to while it is loaded.  These are kept in Ashita/config/crossbar/charname_charid/bindings/job.xml.  You also have a global default palette, which is kept in Ashita/config/crossbar/charname_charid/bindings/default.xml.  After binding keys, your actual binding is generated by overlaying global defaults, then job defaults, then current palette.  This means that if you want a macro to be available on all jobs, you can bind it under global defaults then leave that macro empty in your job defaults and palettes.  If you want a macro to be available across multiple palettes on the same job, you can bind it under job defaults then leave that macro empty in your palettes.

# Changing Palettes
You can store multiple palettes for the same job by using the **Add Palette** option under the binding menu.  These can be thought of as macro sets, and can be cycled through by holding L2 and pressing L1 to cycle backwards, or by holding R2 and pressing R1 to cycle forwards.
