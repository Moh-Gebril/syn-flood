# SYN Flood Demonstration Tool

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![Language: C](https://img.shields.io/badge/Language-C-green.svg)
![Version: 1.0.0](https://img.shields.io/badge/Version-1.0.0-orange.svg)

A lightweight tool demonstrating TCP SYN flood attack for network security research and education.

## 📋 Overview

This tool demonstrates the SYN flood attack technique, a form of denial-of-service (DoS) attack that exploits the TCP three-way handshake. It sends a continuous stream of TCP SYN packets with randomized source IP addresses to a target system, potentially exhausting its connection resources.

> ⚠️ **IMPORTANT**: This tool is developed for educational and research purposes only. Using this tool against systems without explicit permission is illegal and unethical.

## 🔍 Features

- Send TCP SYN packets with randomized source IP addresses
- Configurable target IP address and port
- Adjustable packet transmission rate
- Real-time statistics display (packets sent, duration, rate)
- Clean termination with Ctrl+C, showing final statistics

## 🛠️ Requirements

- Linux operating system
- GCC compiler
- Root privileges (required for raw socket operations)

## 🚀 Installation

### Clone the repository

```bash
git clone https://github.com/Moh-Gebril/syn-flood.git
cd syn-flood
```

### Build from source

```bash
make
```

This will create the executable in the `bin/` directory.

### Optional: Install system-wide

```bash
make install
```

## 📖 Usage

```bash
sudo ./bin/syn_flood <destination_ip> <port> [packet_delay_ms]
```

### Arguments

- `destination_ip`: Target IP address
- `port`: Target port number (1-65535)
- `packet_delay_ms`: Optional delay between packets in milliseconds (default: 0)

### Example

```bash
sudo ./bin/syn_flood 192.168.1.1 80 5
```

This will start a SYN flood demonstration against 192.168.1.1 on port 80 with a 5ms delay between packets.

## 📊 Output

The tool provides real-time statistics during execution:

```
[INFO] Starting SYN flood attack on 192.168.1.1:80
[INFO] Press Ctrl+C to stop the attack
[INFO] Packets sent: 5000 | Duration: 25 seconds | Rate: 200.00 pps
```

## 🧰 Project Structure

```
.
├── bin/                  # Compiled binary directory
├── build/                # Object files directory
├── headers/              # Header files
│   ├── ip_header.h       # IP header structure
│   ├── network_utils.h   # Network utility functions
│   └── tcp_header.h      # TCP header structure
├── main.c                # Main implementation file
├── network_utils.c       # Network utility implementations
├── LICENSE               # MIT License
├── Makefile              # Build system
└── README.md             # This file
```

## 🧠 Educational Context

This project demonstrates concepts including:
- TCP/IP protocol manipulation
- Raw socket programming
- Network packet construction
- Checksums and packet integrity
- Denial-of-service attack vectors

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgements

This project was inspired by the SEED Labs project (https://seedsecuritylabs.org/), which provides hands-on laboratory exercises for computer security education.

## 👤 Author

**Mohamed Gebril**

---

⚠️ **Disclaimer**: This tool is intended for educational purposes only. Use responsibly and only on systems you own or have permission to test. Unauthorized DoS attacks are illegal and unethical.