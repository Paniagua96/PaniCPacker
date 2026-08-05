<img width="512" height="288" alt="gumroad_1280x720_2" src="https://github.com/user-attachments/assets/ed8e6b34-564f-43fd-91ec-0df873b29fa0" />

**PaniCPacker** is a fast, flexible, lightweight Texture Channel Packer built with Qt. Designed for technical artists and game developers, it allows you to pack, unpack, channel-swap, and manipulate individual texture channels to streamline your PBR workflow.

---

## ✨ Features

* **Individual Channel Packing:** Load specific texture maps directly into **R**, **G**, **B**, and **A** channels.
* **Texture Unpacking:** Import an existing packed texture and split it back into individual channels.
* **Flexible Alpha Support:** Toggle Alpha channel usage on or off depending on target shader requirements (e.g., RGB vs RGBA).
* **Channel Swapping & Inversion:** Easily swap channels around or invert individual channel colors (e.g., rough-to-smooth, or flipping normal map Y-channels).
* **Channel Visualization & Preview:**
  * Real-time preview of the final packed result.
  * Solo / isolate individual channels to inspect specific data masks in grayscale.
* **Export & Overwrite:** Export to standard image formats or quickly overwrite source files during iteration.
* **Non-Square Power of Two (Po2) Scaling:** Force export resolutions to Power of Two formats, supporting non-square ratios (e.g., 512x1024, 2048x1024).

---

## 🛠️ Installation & Usage

### Running the Executable
1. Download the latest release from the: ---------
2. Extract the archive and run `Setup_PaniCPacker.exe`.

> **Note:** Keep the accompanying `.dll` files in the same directory as the executable.

---

## 📄 License & Open Source Compliance

PaniCPacker is open-source software built using the **Qt Framework**.

* **Qt Framework:** Covered under the **GNU Lesser General Public License v3 (LGPLv3)**.
* **Relinking & Source Access:** Qt libraries are dynamically linked (`.dll`). Copies of the LGPLv3 license, GPLv3 license, and a written offer for Qt source code access are included in the distribution files as required by the LGPLv3 license.

For more details on Qt licensing, visit [qt.io/licensing](https://www.qt.io/licensing/).


