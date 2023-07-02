# arduino-digital-alarm-clock

Project Summary: Used the PCM library from the arduino libraries, to playback a sample of a sound downloaded from the internet. Amplified this signal using a LM380N amplifier. Current time can be set by the user using the four push-buttons, and the RTC module keeps the time even while there is no power. You can set the time the alarm should trigger in the same way that you set the time.

Pushbutton (from left to right):
- Click to set time or set alarm, when on the set alarm or set clock screen.
- Click to toggle hour, when on the set alarm or set clock screen. Or to delete alarm, when on the alarm check screen.
- Click to togggle minute, when on the set alarm or set clock screen.
- Click to toggle screen/mode, between four different modes. Start at clock mode, then set clock mode, then add alarm mode, and then check/delete alarm mode, and then back to clock mode.

![image](https://github.com/visnjicm/arduino-digital-alarm-clock/assets/126916558/ec60e6d2-c860-49ff-9e42-99e0840c2643)


Things to add/change:
1. Volume control
2. Save alarm to EEPROM, so that the alarm is saved even after you turn off the alarm clock.
3. Achieve at least 4 W of output RMS power on 3-5 W speaker.
4. Add sleep mode?
5. Power analysis, review how much power the system consumes currently, see how much that costs per year (electricity bill) and if necessary see how you can minimize this.
6. Reduce cost of overall system by carefully selecting cheapest components that still achieved all desired functionality.
   - For example: Replace arduino uno with a cheaper microcontroller that has all of the same functionality.
   - For example: Build RTC circuit from scratch instead of purchasing pre-made breakout board.
7. Convert arduino code to full register level code, or at least, as much as possible. (Good for embedded practice).
   - For example: Instead of using library to control LCD, create the same functionality from scratch by looking how to control     the LCD based on datasheets.
