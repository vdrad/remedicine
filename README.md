# REMEDICINE: A Medication Reminder Device

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

![REMEDICINE Alert](https://drive.google.com/thumbnail?id=1C7P-uWIYIyqPwf6WcwN2sv2-_8aM6DUe&sz=w1000)

## 🗒️ Table of Contents
- [About](#-about)
    - [Functionalities](#️-functionalities)
    - [Folder Structure](#-folder-structure)
    - [Results](#-results)
    - [Personal Outcomes](#-personal-outcomes)
    - [Gallery](#️-gallery)
- [License](#️-license)
- [Feedback and Contributions](#-feedback-and-contributions)
- [Contact](#️-contact)

---

## 🔍️ About

> This was my final project for the EMBARCATECH program, an Embedded Systems profissionalization program held by the Brazilian Ministry of Science, Technology, and Innovation.

Medication non-adherence is a critical health issue worldwide. Studies show that about **50% of people with chronic diseases do not follow their treatment correctly** after six months, leading to severe health complications and increased mortality rates. Forgetting to take medication is a major contributor to this problem, especially among the elderly population.

But... *what if we could create an accessible, intuitive, and highly customizable device to help people remember their medications?* **That's exactly what this project aimed to do**.

### 🚀 Project Description
REMEDICINE is a standalone medication reminder device built on the **BitDogLab v6.3** development board. It provides a robust and user-friendly solution to schedule and receive medication alerts.

The system is designed with **accessibility** in mind, offering multiple forms of interaction and notification to cater to a wide range of users, including those with sensory impairments.

The core functionality is straightforward:
1.  **Configuration**: The user registers each medication in a simple configuration file (`remedios.c`). For each reminder, they can define the medicine's name, a unique visual pattern and color for the RGB LED matrix, a specific melody, and the exact date and time for the alert.
2.  **Operation**: Once the configuration is loaded, the device displays the current time on its OLED screen.
3.  **Alerts**: When a scheduled time is reached, REMEDICINE triggers a multi-sensory alarm:
    * The **OLED display** shows the name of the medication.
    * The **5x5 RGB LED matrix** lights up with the pre-configured pattern and color.
    * The **buzzer** plays the custom melody associated with that specific medicine.
4.  **Interaction**: The user can acknowledge the alarm through two methods:
    * **Tactile**: Pressing one of the physical buttons.
    * **Non-Tactile**: Blowing into the onboard microphone.

Successful acknowledgment is confirmed with a green smiling face on the LED matrix and a cheerful tune. If an alert is missed, the device displays a red sad face and plays a distinct, low-pitched melody. Users can also review all scheduled reminders at any time.

---

### ⚙️ Functionalities

* **Customizable Multi-Sensory Alerts**: Provides unique visual (text, pattern, color) and audible (melody) signals for each medication.
* **Accessible Interaction**: Offers both tactile (buttons) and non-tactile (microphone) ways to interact with the device, ensuring usability for diverse needs.
* **High-Capacity Storage**: Capable of storing up to 256 independent medication reminders.
* **Real-Time Clock**: An integrated Real-Time Clock (RTC) accurately keeps track of time to trigger alerts precisely.
* **Ready-to-Go Solution**: The entire system is built using the native components of the BitDogLab board, requiring no external hardware or complex setup.
* **User Feedback**: Provides clear visual and audible feedback for user actions, such as successfully acknowledging an alarm or missing one.

---

### 📂 Folder Structure
The software is organized into modules, each responsible for a specific hardware peripheral or functionality.

```
.
├── remedicine.c            # Main application loop and initialization
|
├── inc
│   ├── alarm               # Manages alarm logic and RTC
│   ├── buttons             # Handles button inputs
│   ├── buzzer              # Controls the buzzer for melodies
│   ├── microphone          # Handles microphone input
│   ├── oled_display        # Controls the OLED display
│   ├── remedios            # User-defined medication schedules and alerts
│   └── rgb_matrix          # Controls the 5x5 RGB LED Matrix  

```


---

### ✨ Results
The project successfully met all its objectives, resulting in a fully functional and reliable medication reminder device. Validation tests confirmed that all hardware peripherals and software modules operate as expected.

The key achievement of REMEDICINE is its focus on **inclusivity and accessibility**. By providing diverse and customizable sensory outputs and inputs, the device is a practical solution for a broad audience, setting it apart from other existing solutions that often have limited notification options. The system proved to be robust, easy to configure, and effective in delivering timely and noticeable alerts.

---

### 📝 Personal Outcomes

This project was an excellent opportunity to dive deep into embedded systems development using the C language. It involved the complete lifecycle of product creation, from conceptualization and hardware selection to software architecture and implementation.

Key learning experiences included:
* **Software Architecture**: Designing a modular and scalable software structure to manage complexity and facilitate future maintenance.
* **Problem-Solving**: Overcoming hardware limitations and debugging complex interactions between different parts of the system to create a seamless user experience.

Overall, REMEDICINE was a challenging yet highly rewarding project that solidified my skills in embedded C programming and hardware-software integration.

---

### 🖼️ Gallery

![Alert for DIPIRONA](https://drive.google.com/thumbnail?id=1HSDEMtVNlJoYi_MZ8IC966LJFS9SgHAz&sz=w1000)
*Example of an alert for "DIPIRONA", showing the custom pattern on the LED matrix and the name on the OLED screen.*

![Success and Failure Messages](https://drive.google.com/thumbnail?id=1rXL1d6fJvn8WPw3sDSnqjfN6SJkfKUf_&sz=w1000)
*Feedback screens for acknowledging an alert (left, in green) and missing an alert (right, in red).*

![Pinout Diagram](https://drive.google.com/thumbnail?id=1Ykru4VWl-hm7cLnTNsc-t9Qw0NPeu4Z-&sz=w1000)
*The complete hardware circuit and pinout for the REMEDICINE device on the Raspberry Pi Pico W board.*

---

## ⚖️ License
This project is licensed under the Apache License 2.0. You are free to use, modify, and distribute this code, provided that proper attribution is given, and the license terms are included in any distributions.

---

## 🤝 Feedback and Contributions
Since REMEDICINE was developed as a final academic project, there are no immediate plans for further improvements. However, you are welcome to explore the code, experiment with it, and share any suggestions or enhancements! The project can be found on [GitHub](https://github.com/vdrad/remedicine).

---

## 💬 Contact
[
    <img
        src="https://images.weserv.nl/?url=https://avatars.githubusercontent.com/u/145018309?v=4&fit=cover&mask=circle&maxage=7d"
        width=15%
        title="GitHub Profile"
        alt="vdrad"
    />
](https://github.com/vdrad)

For questions, suggestions, collaboration opportunities, or any other topic, please feel free to reach out:

-   **GitHub**: [vdrad](https://github.com/vdrad)
-   **Email**: victor.drad.g@gmail.com
-   **LinkedIn**: [Victor Gomes](https://www.linkedin.com/in/victor-g-582b5911b/)