# Armagedom - Networked Space Battleship

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Sockets](https://img.shields.io/badge/Socket%20Programming-Networking-red?style=for-the-badge)

## 📋 Project Overview

**Armagedom** is a multiplayer strategy game developed in C that re-imagines the classic Battleship in a space setting. Unlike simple console games, this project implements **Network Sockets** to enable real-time communication between two players over the internet/LAN.

**Engineering Focus:**
This project demonstrates proficiency in **Systems Programming** on Linux. It handles low-level memory management, file parsing for map configuration, and synchronous network communication between client instances.

## 🚀 Key Features

* **Multiplayer Networking:** Implements socket programming to establish connections between players (Client-Server or Peer-to-Peer logic).
* **Custom Data Structures:** Manages complex, irregular grid shapes for "Islands" (ships), including Triangles, Squares, and L-shapes, rather than standard linear blocks.
* **Dynamic Configuration:** Parses external text files (`planeta.txt`) to load diverse map layouts and planetary environments.
* **Linux Native:** Optimized for the Linux kernel environment, utilizing OS-specific libraries for stable execution.

## 🎮 Game Logic & Theme

The game replaces the traditional "Sea and Ships" with a "Space and Planets" theme:
* **Context:** Players position "Islands" on their "Planets" and launch attacks.
* **Entities:**
    * *Triangular Island* (3 blocks)
    * *Square Island* (2x2 blocks)
    * *Principal Island* (Critical target)
* **Feedback System:** The engine returns specific destruction codes (e.g., `Zp` for Principal Island, `Zt` for Triangular) to confirm hits.

## 🛠️ Tech Stack

* **Language:** C (C99 Standard)
* **OS:** Linux (Required for Socket libraries)
* **Networking:** TCP/IP Sockets
* **I/O:** File Stream Processing

## 💻 How to Run (Linux Only)

Since this project uses Linux-specific headers, it must be compiled in a Linux environment (Ubuntu, Debian, WSL, etc.).

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/RafahelMatias/batalha-naval-c.git](https://github.com/RafahelMatias/batalha-naval-c.git)
    cd batalha-naval-c
    ```

2.  **Compile the Engine:**
    ```bash
    gcc main.c -o armagedom
    ```

3.  **Run the Game:**
    * *Player 1 (Host/Server):*
        ```bash
        ./armagedom [OPPONENT_IP]
        ```
    * *Player 2:*
        ```bash
        ./armagedom [OPPONENT_IP]
        ```
    *(Note: Refer to the source code for specific connection arguments).*

---
**Author:** Rafahel Matias
*Software Engineer specializing in Backend Development & Automation.*