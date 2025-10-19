# GoL Ncurses TUI

A classic implementation of Conway's Game of Life (GoL) as a Terminal User Interface (TUI), written in C and utilizing the ncurses library.

---

## Overview

GoL Ncurses TUI is a simple, high-performance simulation designed to run directly in your terminal. It leverages the **ncurses library** to provide a dynamic, responsive grid and a status bar, showcasing efficient memory management and classic C programming structure. The simulation features toroidal (wrapping) boundaries and various seeding patterns.

---

## Features

* **Terminal User Interface (TUI):** Runs entirely within the console using ncurses.
* **Toroidal Grid:** Cells wrap around the screen edges (top-to-bottom, left-to-right).
* **Efficient Update Logic:** Uses two grids (`current_grid` and `next_grid`) for rapid, safe generation updates.
* **Pattern Seeding:** Seeds the world with various stable and chaotic starting patterns (e.g., Glider, R-Pentomino).
* **Dynamic Resizing:** Supports terminal resizing on the fly (`KEY_RESIZE`).
* **Simple Controls:** Quit, Pause, and Step-by-Step generation advance.

---

## Requirements

* **C Compiler:** A standard C compiler (GCC or Clang).
* **GNU Make:** For building the project with the provided `Makefile`.
* **ncurses Library:** The development files for the ncurses library (e.g., `libncurses-dev` on Debian/Ubuntu, `ncurses-devel` on Fedora/CentOS, or installed via package manager on macOS/Arch Linux).

**Supported Systems:** Linux, macOS, and other Unix-like environments.

---

## Installation and Build

### Using Git (Recommended)

1.  Clone the repository:
  ```bash
  git clone [https://github.com/GrunclePug/gol.git](https://github.com/GrunclePug/gol.git)
  cd gol
  ```
2.  Compile the project using `make`:
  ```bash
  make
  ```
3.  Run the executable:
  ```bash
  ./gol
  # Optional: Run with a custom seeding density (0-1000, default is 35)
  # ./gol 10
  ```

### System-Wide Installation (make install)

To install the executable to a directory included in your system's PATH (defaulting to /usr/local/bin/ as defined in config.mk), use make install. This allows you to run the program simply by typing gol.
```bash
# You will likely need root privileges for this step
sudo make install
```

### Uninstallation

To remove the executable from your system:

```bash
# You will need root privileges for this step
sudo make uninstall
```

### From a Source Tarball

1.  Download the latest release tarball (`gol-X.X.tar.gz`).
2.  Extract the files:
  ```bash
  tar -xzf gol-X.X.tar.gz
  cd gol-X.X
  ```
3.  Compile and run using the steps above.

---

## Configuration

Default settings (like speed, minimum terminal size, and keys) are defined in **`config.h`**.

1.  Copy the default configuration (this is done by default on build if `config.h` doesn't exist):
  ```bash
  cp config.def.h config.h
  ```
2.  Edit `config.h` to customize the values:
  ```bash
  # For example, to change the simulation speed:
  #define DEFAULT_SPEED_MS 50
  ```
3.  Recompile the project:
  ```bash
  make clean
  make
  ```

---

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and submit a pull request.

---

## Author

Chad Humphries |
[Website](https://grunclepug.com/) |
[GitHub Profile](https://github.com/GrunclePug)

---

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
