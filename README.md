# arduino-digital-alarm-clock
![image](https://github.com/visnjicm/arduino-digital-alarm-clock/assets/126916558/ec60e6d2-c860-49ff-9e42-99e0840c2643)


Things to add/change:
1. Volume control
2. Achieve at least 4 W of output RMS power on 3-5 W speaker.
3. Add sleep mode?
4. Power analysis, review how much power the system consumes currently, see how much that costs per year (electricity bill) and if necessary see how you can minimize this.
5. Reduce cost of overall system by carefully selecting cheapest components that still achieved all desired functionality.
   - For example: Replace arduino uno with a cheaper microcontroller that has all of the same functionality.
   - For example: Build RTC circuit from scratch instead of purchasing pre-made breakout board.
6. Convert arduino code to full register level code, or at least, as much as possible. (Good for embedded practice).
   - For example: Instead of using library to control LCD, create the same functionality from scratch by looking how to control     the LCD based on datasheets.
