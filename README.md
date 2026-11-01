
STM32 Bluetooth Password Security System
A secure password-protected access control system built using an STM32 microcontroller, Bluetooth module, OLED display, LEDs, and buzzer. Users can enter a password wirelessly through a Bluetooth-connected device, and the system provides visual and audio feedback for authentication results.

📌 Features
Bluetooth-based password authentication
OLED display for system status and messages
Correct password access control
Automatic relocking after 7 seconds
Security lockout after multiple failed attempts
Hidden recovery password mechanism
Green LED indication for successful login
Red LED warning for incorrect passwords
Buzzer sound patterns for different system states
🛠 Components Used
STM32 Development Board
HC-05 / HC-06 Bluetooth Module
SSD1306 OLED Display (128x64)
Green LED
Red LED
Active Buzzer
Jumper Wires
⚙️ Working
The system starts in a locked state.
Users enter a password through a Bluetooth-connected device.
If the password is correct:
Green LED flashes.
Success buzzer pattern is played.
System unlocks for 7 seconds.
If the password is incorrect:
Red LED flashes.
Error buzzer sounds.
Failed attempt counter increases.
After 3 wrong attempts:
System enters lockout mode.
Only the recovery password can unlock the system.
After 7 seconds of successful access:
System automatically relocks.
🔌 Pin Configuration
Component	STM32 Pin
Green LED	PB13
Red LED	PB12
Buzzer	PB14
OLED SDA	PB7
OLED SCL	PB6
Bluetooth TX/RX	PA9 / PA10
💻 Software Used
Arduino IDE
STM32 Arduino Core
Adafruit GFX Library
Adafruit SSD1306 Library
🚀 Future Improvements
Mobile app integration
Fingerprint authentication
RFID card support
Password change feature
Event logging system
👨‍💻 Author
Elbin Sam
