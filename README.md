Smart Bluetooth Door Lock System using STM32
A smart security system built with an STM32 microcontroller that enables wireless password authentication through Bluetooth communication. The project combines an OLED display, LEDs, and buzzer-based alerts to provide a secure and user-friendly access control solution.

🎯 Project Overview
This system allows users to unlock access by sending a password via a Bluetooth-connected device. The entered password is verified by the STM32 microcontroller, and the system responds with visual and audio feedback. To improve security, multiple failed login attempts trigger a lockout mode that can only be cleared using a hidden recovery password.

✨ Features

📱 Bluetooth-based wireless authentication
🔐 Password-protected access control
🖥 OLED display for status monitoring
✅ Success and error notifications
🚫 Automatic lockout after three failed attempts
🔑 Recovery password support
⏳ Automatic relocking after successful login
🔊 Custom buzzer alerts for different system conditions
💡 LED indicators for system status
🧰 Hardware Components

STM32 Development Board

HC-05 / HC-06 Bluetooth Module
SSD1306 OLED Display
Active Buzzer
Red LED
Green LED
Jumper Wires

🔄 System Workflow

User sends a password through Bluetooth.
STM32 verifies the received password.
If correct:
Access is granted.
Green LED flashes.
Success buzzer sounds.
System remains unlocked for 7 seconds.
If incorrect:
Error message is displayed.
Red LED flashes.
Failed attempt counter increases.
After three failed attempts:
Lockout mode is activated.
Only a recovery password can restore access.
System automatically locks again after the unlock timer expires.

💻 Technologies Used

C++
Arduino IDE
STM32 Arduino Core
Adafruit GFX Library
Adafruit SSD1306 Library
Bluetooth Serial Communication

🎓 Learning 

This project demonstrates:

Embedded Systems Programming
Bluetooth Communication
Serial Data Processing
Security Mechanisms
OLED Display Integration
Peripheral Control using STM32

👨‍💻 Author
Elbin Sam

B.Tech Cyber Security Student
College of Engineering, Kallooppara
