# arduino-digital-alarm-clock
![Untitled Sketch_bb](https://github.com/visnjicm/arduino-digital-alarm-clock/assets/126916558/180d9295-7432-49b9-831c-bed49820a357)

Things to add/change:
1. Volume control
2. Achieve at least 4 W of output RMS power on 3-5 W speaker.
3. Add sleep mode?
4. Reduce cost of overall system by carefully selecting cheapest components that still achieved all desired functionality.
   - For example: Replace arduino uno with a cheaper microcontroller that has all of the same functionality.
   - For example: Build RTC circuit from scratch instead of purchasing pre-made breakout board.
5. Convert arduino code to full register level code, or at least, as much as possible. (Good for embedded practice).
   - For example: Instead of using library to control LCD, create the same functionality from scratch by looking how to control     the LCD based on datasheets.
